//
// Created by Akira Mujawar on 19/10/24.
//

#ifndef MEOWENGINE_APPLICATION_TEST_HPP
#define MEOWENGINE_APPLICATION_TEST_HPP

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

#include <frame_rate_counter.hpp>
#include "sdl_window.hpp"
#include "SDL_image.h"
#include "thread_barrier.hpp"
#include "queue"
#include "double_buffer.hpp"
#include "entt_reflection_wrapper.hpp"


#include "opengl_physics_thread.hpp"
#include "opengl_render_thread.hpp"


using namespace std;

namespace MeowEngine {

    class ApplicationTest {
    public:
        ApplicationTest() {}
        virtual ~ApplicationTest() {}

        void StartApplication() {
            IsApplicationRunning = true;

#ifdef __EMSCRIPTEN__
            //  emscripten_set_main_loop(emscriptenLoop, 60, 1);
            emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
#else

            MeowEngine::Log("Main Thread", "SDL2 Initialized");
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
                throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
            }

            if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
                throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
            }

            ProcessThreadBarrier = std::make_shared<ThreadBarrier>(2);
            SwapBufferThreadBarrier = std::make_shared<ThreadBarrier>(2);

            RenderThread = std::make_unique<MeowEngine::OpenGLRenderThread>();
            PhysicThread = std::make_unique<MeowEngine::OpenGLPhysicsThread>();

            InputManager = std::make_unique<MeowEngine::input::InputManager>();

            Scene = std::make_shared<MeowEngine::MainScene>(MeowEngine::sdl::GetWindowSize(RenderThread->WindowContext->window));

            // NOTE: Clearing context in main thread before using for render thread fixes a crash
            // which occurs while drag window
            SDL_GL_MakeCurrent(RenderThread->WindowContext->window, nullptr);

            MainThreadFrameRate = std::make_unique<FrameRateCounter>(60, 1); // 60 frames per second

            RenderThread->StartThread(*this);
            PhysicThread->StartThread(*this);

            MainThreadLoop();

            RenderThread->EndThread();
            PhysicThread->EndThread();

            InputManager.reset();

            MeowEngine::Log("Application", "Ended");
#endif
        }

        // multithreading
        std::atomic<bool> IsApplicationRunning;
        std::atomic<int> ThreadCount;
        std::condition_variable WaitForThreadEndCondition;
        std::mutex WaitForThreadEndMutex;
        std::atomic<bool> IsSyncingPhysicsThread;
        std::mutex SyncPhysicMutex;
        std::unique_ptr<FrameRateCounter> MainThreadFrameRate;

        std::shared_ptr<ThreadBarrier> ProcessThreadBarrier;
        std::shared_ptr<ThreadBarrier> SwapBufferThreadBarrier;

        MeowEngine::DoubleBuffer<std::queue<SDL_Event>> InputBuffer = MeowEngine::DoubleBuffer<std::queue<SDL_Event>>();

        std::unique_ptr<MeowEngine::OpenGLPhysicsThread> PhysicThread;
        std::unique_ptr<MeowEngine::OpenGLRenderThread> RenderThread;


        void MainThreadLoop() {
            // init
            PT_PROFILE_SCOPE;
            MeowEngine::Log("Main Thread", "Started");

            Scene->CreateSceneOnMainThread();

//            MeowEngine::Log("Main Thread", "waiting for creating all threads");
//            std::unique_lock<std::mutex> lock(WaitForThreadEndMutex);
//            WaitForThreadEndCondition.wait(lock, [this] { return ThreadCount == 0;});
//
//            MeowEngine::Log("Main Thread", "Created");

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // loop
            while(IsApplicationRunning)
            {
                MainThreadFrameRate->Calculate();

                PT_PROFILE_SCOPE;
                // if thread count == 0 on main notify

                // update thread count++

//                MeowEngine::Log("Main Thread", "Running");
//                MeowEngine::Log("Frame Rate: ", static_cast<float>(MainThreadFrameRate->DeltaTime));
                Update(MainThreadFrameRate->DeltaTime);

                if(!Input(MainThreadFrameRate->DeltaTime))
                {
                    PT_PROFILE_SCOPE_N("Main Thread Ending");
                    IsApplicationRunning = false;

                    // if threads are waiting for thread sync we unpause them
                    ProcessThreadBarrier->End();
                    SwapBufferThreadBarrier->End();
                    MeowEngine::Log("Main Thread", "Ending");
                    break;
                }

//                std::this_thread::sleep_for(std::chrono::milliseconds(100));
//                MeowEngine::Log("Main Thread", "waiting process");

                // - frames updating (processing / rendering)
                //      - wait for all threads to finish processing the frame
                // - swap buffers
                // - merge data
                //      - wait for swapping and merging to finish
                // - continue for new frame

                // wait for all threads to sync up for frame ending

                ProcessThreadBarrier.get()->Wait();


//                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                // swap buffers
                InputBuffer.Swap();

                // swap
                // delta add to final rigidbody all frames after main thread calculations
                // delta is buffered 3 ways
                // write from main thread on this
                // read from render thread on this

                // staging accesses final rigidbody takes the delta
                if(SyncPhysicMutex.try_lock()) {
                    Scene->SyncPhysicsBufferOnMainThread(false);
                    SyncPhysicMutex.unlock();
                }
                else {
                    Scene->SyncPhysicsBufferOnMainThread(true);
                }


                Scene->SyncRenderBufferOnMainThread();

                Scene->SwapMainAndRenderBufferOnMainThread();

                MainThreadFrameRate->LockFrameRate();

                // wait until buffers are synced
                // ideally since other threads are waiting this should release all of them automatically
                SwapBufferThreadBarrier.get()->Wait();
            }

            MeowEngine::Log("Main Thread", "Waiting for other threads to end");

            std::unique_lock<std::mutex> lock(WaitForThreadEndMutex);
            WaitForThreadEndCondition.wait(lock, [this] { return ThreadCount == 0;});

            MeowEngine::Log("Main Thread", "Ended");
        }

        // main thread ----------------

        std::unique_ptr<MeowEngine::input::InputManager> InputManager;
        std::shared_ptr<MeowEngine::Scene> Scene;

        bool Input(const float& deltaTime) {
            PT_PROFILE_SCOPE;
            SDL_Event event;

//            while(!InputBuffer.GetCurrent().empty())
//            {
//                event = InputBuffer.GetCurrent().front();
//                InputBuffer.GetCurrent().pop();
//
//                // perform the task
//                MeowEngine::Log("InputBuffer", "Event");
//            }

            // Each loop we will process any events that are waiting for us.
            while (SDL_PollEvent(&event))
            {
                InputBuffer.GetCurrent().push(event);

                switch (event.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        InputManager->SetMouseDown();
                        break;
                    case SDL_WINDOWEVENT:
//                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
//                        OnWindowResized();
//                    }
                        break;

                        // If we get a quit signal, we will return that we don't want to keep looping.
                    case SDL_QUIT:
                        RenderThread->UI->ClosePIDs();
                        return false;

                    case SDL_KEYDOWN:
                        // If we get a key down event for the ESC key, we also don't want to keep looping.
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            RenderThread->UI->ClosePIDs();
                            return false;
                        }
                        break;

                    case SDL_USEREVENT:
                        switch (event.user.code) {
                            case 2: {
                                MeowEngine::Log("Main Thread", "Rescaled Window");

                                const WindowSize size = *(WindowSize *) event.user.data1;
                                Scene->OnWindowResized(size);

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

            Scene->Input(deltaTime, *InputManager.get());
            InputManager->isMouseDown = false;
            return true;
        };

        void Update(const float& deltaTime) {
            PT_PROFILE_SCOPE;
            Scene->Update(deltaTime);
        };

    };
}


#endif //MEOWENGINE_APPLICATION_TEST_HPP
