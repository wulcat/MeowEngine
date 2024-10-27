//
// Created by Akira Mujawar on 19/10/24.
//

#ifndef PHYSICAT_APPLICATION_TEST_HPP
#define PHYSICAT_APPLICATION_TEST_HPP

#include "thread"
#include <string>

#include "graphics_wrapper.hpp"
#include "sdl_wrapper.hpp"
#include "log.hpp"

#include "opengl_renderer.hpp"
#include "imgui_renderer.hpp"

#include "opengl_framebuffer.hpp"
#include "opengl_asset_manager.hpp"
#include "input_manager.hpp"
#include "main_scene.hpp"
#include "physx_physics.hpp"
#include <fps_counter.hpp>
#include "window.hpp"
#include "SDL_image.h"
#include "custom_barrier.hpp"

using namespace std;

namespace physicat {
    struct ApplicationTest {
    public:
        ApplicationTest() {}
        virtual ~ApplicationTest() {}

        void StartApplication() {
            IsApplicationRunning = true;

#ifdef __EMSCRIPTEN__
            //  emscripten_set_main_loop(emscriptenLoop, 60, 1);
            emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
#else

            physicat::Log("Main Thread", "SDL2 Initialized");
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
                throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
            }

            if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
                throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
            }

            Barrier = std::make_shared<CustomBarrier>(2);
            WindowContext = std::make_unique<physicat::Window>();
            AssetManager = std::make_shared<physicat::OpenGLAssetManager>(physicat::OpenGLAssetManager());
//            Renderer = std::make_unique<physicat::OpenGLRenderer>(AssetManager);
            UI = std::make_unique<physicat::graphics::ImGuiRenderer>(WindowContext->window, WindowContext->context);
//            FrameBuffer = std::make_unique<physicat::graphics::OpenGLFrameBuffer>(1000,500);
            InputManager = std::make_unique<physicat::input::InputManager>();
//            Physics = std::make_shared<physicat::simulator::PhysXPhysics>();

            // NOTE: Clearing context in main thread before using for render thread fixes a crash
            // which occurs while drag window
            SDL_GL_MakeCurrent(WindowContext->window, nullptr);

            // create threads and start keep infinite thread
            RenderThread = std::thread(&physicat::ApplicationTest::RenderThreadLoop, this);
            PhysicsThread = std::thread(&physicat::ApplicationTest::PhysicsThreadLoop, this);
            MainThreadLoop();

            // stop threads
            RenderThread.join();
            PhysicsThread.join();

            Physics.reset();
            InputManager.reset();

            AssetManager.reset();

//            FrameBuffer.reset();
            UI.reset();
//            Renderer.reset();
            WindowContext.reset();

            physicat::Log("Application", "Ended");
#endif
        }

//        bool LoopApplication() {}

        // multithreading
        std::atomic<bool> IsApplicationRunning;
        std::atomic<int> ThreadCount;
        std::condition_variable WaitForThreadEndCondition;
        std::mutex WaitForThreadEndMutex;
        std::shared_ptr<CustomBarrier> Barrier;

        void MainThreadLoop() {
            // init
            physicat::Log("Main Thread", "Started");

            // loop
            while(IsApplicationRunning)
            {
                // if thread count == 0 on main notify

                // update thread count++

                physicat::Log("Main Thread", "Running");

                Update(0);

                if(!Input(0))
                {
                    IsApplicationRunning = false;
                    physicat::Log("Main Thread", "Ending");
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                // reduce thread count
                Barrier.get()->Wait();
            }

            std::unique_lock<std::mutex> lock(WaitForThreadEndMutex);
            WaitForThreadEndCondition.wait(lock, [this] { return ThreadCount == 0;});
            physicat::Log("Main Thread", "Ended");
            // exit


        }

        void RenderThreadLoop() {
            // init
            physicat::Log("Render Thread", "Started");
            ThreadCount++;

            SDL_GL_MakeCurrent(WindowContext->window, WindowContext->context);

            // loop
            while (IsApplicationRunning) {
                PT_PROFILE_SCOPE;
                // Synchronize with the main thread
                // Lock mutex, get game state updates, unlock
                physicat::Log("Render Thread", "Running");

                // Clear frame
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Issue OpenGL draw calls

                // Swap buffers
                SDL_GL_SwapWindow(WindowContext->window);

                Render();
                Barrier.get()->Wait();
            }

            // exit
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            ThreadCount--;

            physicat::Log("Render Thread", "Ended");
            WaitForThreadEndCondition.notify_all();
        }

        std::thread PhysicsThread;

        void PhysicsThreadLoop() {
            physicat::Log("Physics Thread", "Started");
            ThreadCount++;

            while(IsApplicationRunning)
            {
                PT_PROFILE_SCOPE;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
//                Barrier.get()->Wait();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            ThreadCount--;

            physicat::Log("Physics Thread", "Ended");
            WaitForThreadEndCondition.notify_all();
        }

        // main thread ----------------

        std::unique_ptr<physicat::input::InputManager> InputManager;
        std::shared_ptr<physicat::Scene> Scene;


        bool Input(const float& deltaTime) {
            PT_PROFILE_SCOPE;
            SDL_Event event;

            // Each loop we will process any events that are waiting for us.
            while (SDL_PollEvent(&event))
            {
                UI->Input(event);

                switch (event.type)
                {
                    case SDL_WINDOWEVENT:
//                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
//                        OnWindowResized();
//                    }
                        break;

                        // If we get a quit signal, we will return that we don't want to keep looping.
                    case SDL_QUIT:
                        UI->ClosePIDs();
                        return false;

                    case SDL_KEYDOWN:
                        // If we get a key down event for the ESC key, we also don't want to keep looping.
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            UI->ClosePIDs();
                            return false;
                        }
                        break;

                    case SDL_USEREVENT:
                        switch (event.user.code) {
                            case 2: {
                                const WindowSize size = *(WindowSize *) event.user.data1;
//                                OnViewportResize(size);
                                break;
                            }
                            case 3: {
                                InputManager->isActive = *(bool *) event.user.data1;
                                break;
                            }
                        }
                    default:
                        break;
                }
            }

            // Track keyboard and mouse clicks/hold/drag/position
            InputManager->ProcessInput();

//            GetScene().Input(deltaTime, InputManager);

            return true;
        };

        void Update(const float& deltaTime) { };

        // render ----------------
        std::thread RenderThread;
        // we decouple window / context into a class
        std::unique_ptr<physicat::Window> WindowContext;
        std::unique_ptr<physicat::graphics::ImGuiRenderer> UI;
        std::unique_ptr<physicat::OpenGLRenderer> Renderer;
        std::unique_ptr<physicat::graphics::OpenGLFrameBuffer> FrameBuffer;
        // this is shared because even main thread will access asset manager and sometimes physics
        // but render thread will access this all the time
        std::shared_ptr<physicat::OpenGLAssetManager> AssetManager;

        void Render() { };

        // physics thread ----------------
        std::shared_ptr<physicat::simulator::Physics> Physics;

        void FixedUpdate(const float& inFixedDeltaTime) {};

        static FpsCounter FpsCounter;
    };
}


#endif //PHYSICAT_APPLICATION_TEST_HPP
