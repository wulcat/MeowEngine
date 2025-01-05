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
#include "thread_barrier.hpp"
#include "queue"
#include "double_buffer.hpp"

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

            ProcessThreadBarrier = std::make_shared<ThreadBarrier>(2);
            SwapBufferThreadBarrier = std::make_shared<ThreadBarrier>(2);

            WindowContext = std::make_unique<physicat::Window>();
            AssetManager = std::make_shared<physicat::OpenGLAssetManager>(physicat::OpenGLAssetManager());
            Renderer = std::make_unique<physicat::OpenGLRenderer>(AssetManager);
            UI = std::make_unique<physicat::graphics::ImGuiRenderer>(WindowContext->window, WindowContext->context);
            FrameBuffer = std::make_unique<physicat::graphics::OpenGLFrameBuffer>(1000,500);
            InputManager = std::make_unique<physicat::input::InputManager>();
//            Physics = std::make_shared<physicat::simulator::PhysXPhysics>();

            Scene = std::make_shared<physicat::MainScene>(physicat::sdl::GetWindowSize(WindowContext->window));

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

            FrameBuffer.reset();
            UI.reset();
            Renderer.reset();
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

        std::shared_ptr<ThreadBarrier> ProcessThreadBarrier;
        std::shared_ptr<ThreadBarrier> SwapBufferThreadBarrier;

        //WindowSize sizeTemp;
//        std::queue<SDL_Event> inputQueue;
        physicat::DoubleBuffer<std::queue<SDL_Event>> InputBuffer = physicat::DoubleBuffer<std::queue<SDL_Event>>();
//        physicat::DoubleBuffer<int> Test = physicat::DoubleBuffer<int>();
        // ecs -> double buffer template -> command buffer

        void MainThreadLoop() {
            // init
            physicat::Log("Main Thread", "Started");
            Scene.get()->Create(Physics);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // loop
            while(IsApplicationRunning)
            {
                // if thread count == 0 on main notify

                // update thread count++

//                physicat::Log("Main Thread", "Running");

                Update(0);

                if(!Input(0.02f))
                {
                    IsApplicationRunning = false;
                    // if threads are waiting for thread sync we unpause them
                    ProcessThreadBarrier->End();
                    SwapBufferThreadBarrier->End();
                    physicat::Log("Main Thread", "Ending");
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
//                physicat::Log("Main Thread", "waiting process");
                // wait for all threads to sync up for frame ending
                ProcessThreadBarrier.get()->Wait();

                // swap buffers
                InputBuffer.Swap();

//                physicat::Log("Main Thread", "waiting swap");
                // wait until buffers are synced
                // ideally since other threads are waiting this should release all of them automatically
                SwapBufferThreadBarrier.get()->Wait();
            }

            physicat::Log("Main Thread", "waiting for closing all threads");
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
            Scene.get()->Load(AssetManager);
//            Renderer = std::make_unique<physicat::OpenGLRenderer>(AssetManager);
            // loop
            while (IsApplicationRunning) {
                PT_PROFILE_SCOPE;
                // Synchronize with the main thread
                // Lock mutex, get game state updates, unlock
//                physicat::Log("Render Thread", "Running");

                // input

                while(!InputBuffer.GetFinal().empty()) {
                    SDL_Event event = InputBuffer.GetFinal().front();
                    InputBuffer.GetFinal().pop();

                    switch (event.type) {
                        case SDL_USEREVENT:
                            switch (event.user.code) {
                                case 2: {
                                    const WindowSize size = *(WindowSize *) event.user.data1;

                                    glViewport(0, 0, size.Width, size.Height);
                                    FrameBuffer->RescaleFrameBuffer(size.Width, size.Height);
                                    physicat::Log("Render Thread", "rescale userevent");
                                    break;
                                }
                            }
                    }
                }

                // Clear frame
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Issue OpenGL draw calls
                Render();

                {
                    PT_PROFILE_SCOPE_N("Swapping GL Buffer");
                    // Swap buffers
                    SDL_GL_SwapWindow(WindowContext->window);
                }
//                physicat::Log("Render Thread", "process waiting");
                // wait for all threads to sync up for frame ending
                ProcessThreadBarrier.get()->Wait();
//                physicat::Log("Render Thread", "swap waiting");
                // wait until buffers are synced on main thread
                SwapBufferThreadBarrier.get()->Wait();

//                glViewport(0,0, sizeTemp.Width,sizeTemp.Height);
//                FrameBuffer->RescaleFrameBuffer(sizeTemp.Width, sizeTemp.Height);
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
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

            while(!InputBuffer.GetFront().empty())
            {
                event = InputBuffer.GetFront().front();
                InputBuffer.GetFront().pop();

                // perform the task
            }

            // Each loop we will process any events that are waiting for us.
            while (SDL_PollEvent(&event))
            {
                InputBuffer.GetCurrent().push(event);

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
                                Scene->OnWindowResized(size);
                                physicat::Log("mian Thread", "rescale userevent");
//                                sizeTemp = size;
//                                glViewport(0,0, size.Width,size.Height);
//                                FrameBuffer->RescaleFrameBuffer(size.Width, size.Height);
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

            Scene.get()->Input(deltaTime, *InputManager.get());

            return true;
        };

        void Update(const float& deltaTime) {
            Scene.get()->Update(10);
        };

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

        void Render() {

            Uint64 currentTime = SDL_GetPerformanceCounter();
            {
                PT_PROFILE_SCOPE_N("setting current");
                // We let opengl know that any after this will be drawn into custom frame buffer
                {
                    PT_PROFILE_SCOPE_N("framebuffer");
                    FrameBuffer.get()->Bind();

                    {
                        PT_PROFILE_SCOPE_N("scene render");
                        glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        Scene.get()->Render(*Renderer);

                        FrameBuffer.get()->Unbind();
                    }
                }

                glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                {
                    PT_PROFILE_SCOPE_N("UI render");
//                    UI.Render(GetScene(), FrameBuffer.GetFrameTexture(), FpsCounter.getSmoothFPS());
                    UI.get()->Render(*Scene.get(), FrameBuffer.get()->GetFrameTexture(), 1);
                }

                {
                    PT_PROFILE_SCOPE_N("waiting");

//
//                    // Frame timing logic
//                    Uint64 frameEndTime = SDL_GetPerformanceCounter();
//                    double frameDuration = (double) (frameEndTime - currentTime) / frequency;
//
//                    while (frameDuration < targetFrameTime) {
//                        frameEndTime = SDL_GetPerformanceCounter();
//                        frameDuration = (double) (frameEndTime - currentTime) / frequency;
//                    }
//
//                    previousTime = frameEndTime;
                }
                {
//                    FpsCounter.frameEnd();
                    PT_PROFILE_SCOPE_N("swap");
//                    SDL_GL_SwapWindow(Window);



//                // Manual Frame Synchronization --
//                // Insert a fence sync object at the end of the previous frame's commands
//                GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
//                // Wait until the fence is signaled, meaning the previous frame is done
//                GLenum result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1e9)); // Timeout in nanoseconds
//
//                // Optionally handle the result to check if it timed out or was successful
//                if (result == GL_WAIT_FAILED) {
//                    // Handle the error
//                }
//
//                // Clean up the sync object after use
//                glDeleteSync(sync);
//                 -- end
                }
            }

        };

        // physics thread ----------------
        std::shared_ptr<physicat::simulator::Physics> Physics;

        void FixedUpdate(const float& inFixedDeltaTime) {};

        static FpsCounter FpsCounter;
    };
}


#endif //PHYSICAT_APPLICATION_TEST_HPP
