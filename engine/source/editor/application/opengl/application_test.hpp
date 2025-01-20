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
#include <frame_rate_counter.hpp>
#include "sdl_window.hpp"
#include "SDL_image.h"
#include "thread_barrier.hpp"
#include "queue"
#include "double_buffer.hpp"
#include "entt_reflection_wrapper.hpp"
//#include "entt_reflection.hpp"

using namespace std;

namespace MeowEngine {
//    extern MeowEngine::EnttReflection ReflectionTest;

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

            MeowEngine::Log("Main Thread", "SDL2 Initialized");
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
                throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
            }

            if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
                throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
            }

            ProcessThreadBarrier = std::make_shared<ThreadBarrier>(2);
            SwapBufferThreadBarrier = std::make_shared<ThreadBarrier>(2);

            WindowContext = std::make_unique<MeowEngine::SDLWindow>();
            AssetManager = std::make_shared<MeowEngine::OpenGLAssetManager>(MeowEngine::OpenGLAssetManager());
            UI = std::make_shared<MeowEngine::graphics::ImGuiRenderer>(WindowContext->window, WindowContext->context);
            Renderer = std::make_unique<MeowEngine::OpenGLRenderer>(AssetManager, UI);

            FrameBuffer = std::make_unique<MeowEngine::graphics::OpenGLFrameBuffer>(1000,500);
            InputManager = std::make_unique<MeowEngine::input::InputManager>();
            Physics = std::make_shared<MeowEngine::simulator::PhysXPhysics>();

            Scene = std::make_shared<MeowEngine::MainScene>(MeowEngine::sdl::GetWindowSize(WindowContext->window));

            // NOTE: Clearing context in main thread before using for render thread fixes a crash
            // which occurs while drag window
            SDL_GL_MakeCurrent(WindowContext->window, nullptr);

            MainThreadFrameRate = std::make_unique<FrameRateCounter>(60, 1); // 60 frames per second
            RenderThreadFrameRate = std::make_unique<FrameRateCounter>(60, 100);
            PhysicsThreadFrameRate = std::make_unique<FrameRateCounter>(50, 1); // per 0.02 sec

            // create threads and start keep infinite thread
            RenderThread = std::thread(&MeowEngine::ApplicationTest::RenderThreadLoop, this);
            PhysicsThread = std::thread(&MeowEngine::ApplicationTest::PhysicsThreadLoop, this);
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

        std::unique_ptr<FrameRateCounter> RenderThreadFrameRate;

        void RenderThreadLoop() {

            // init
            MeowEngine::Log("Render Thread", "Started");
            ThreadCount++;

            SDL_GL_MakeCurrent(WindowContext->window, WindowContext->context);
            Scene->LoadOnRenderThread(AssetManager);

            // loop
            while (IsApplicationRunning) {
//                Uint64 currentTime = SDL_GetPerformanceCounter();
                RenderThreadFrameRate->Calculate();

                PT_PROFILE_SCOPE;
                // Synchronize with the main thread
                // input
                while(!InputBuffer.GetFinal().empty()) {
                    SDL_Event event = InputBuffer.GetFinal().front();
                    InputBuffer.GetFinal().pop();

                    UI->Input(event);

                    switch (event.type) {
                        case SDL_USEREVENT:
                            switch (event.user.code) {
                                case 2: {
                                    const WindowSize size = *(WindowSize *) event.user.data1;

                                    glViewport(0, 0, size.Width, size.Height);
                                    FrameBuffer->RescaleFrameBuffer(size.Width, size.Height);
                                    MeowEngine::Log("Render Thread", "rescale userevent");
                                    break;
                                }
                            }
                    }
                }

                // Clear frame
                //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Issue OpenGL draw calls

                Render();

                {
                    PT_PROFILE_SCOPE_N("Swapping GL Buffer");
                    // Swap buffers
                    SDL_GL_SwapWindow(WindowContext->window);
                }
// Frame timing logic

//                RenderThreadFrameRate->LockFrameRate();
//                MeowEngine::Log("Frequency", ((int)SDL_GetPerformanceFrequency()) * 0.001f);

                // renderring & swapping buffer again gives consistent frames - look into it more for stability
//                Render();
//
//                {
//                    PT_PROFILE_SCOPE_N("Swapping GL Buffer");
//                    // Swap buffers
//                    SDL_GL_SwapWindow(WindowContext->window);
//                }
                // MeowEngine::Log("Render Thread", "Waiting for other threads to finish processing");
                // wait for all threads to sync up for frame ending
                ProcessThreadBarrier.get()->Wait();

                // MeowEngine::Log("Render Thread", "Waiting for main thread to finish swapping buffers");
                // wait until buffers are synced on main thread
                SwapBufferThreadBarrier.get()->Wait();


//                RenderThreadFrameRate.End();
            }

            // exit
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            ThreadCount--;

            MeowEngine::Log("Render Thread", "Ended");
            WaitForThreadEndCondition.notify_all();
        }

        std::thread PhysicsThread;
        std::shared_ptr<MeowEngine::simulator::Physics> Physics;
        std::unique_ptr<FrameRateCounter> PhysicsThreadFrameRate;

        void PhysicsThreadLoop() {
            MeowEngine::Log("Physics Thread", "Started");


            ThreadCount++;
            Physics->Create();


            while(IsApplicationRunning)
            {
                PhysicsThreadFrameRate->Calculate();

                PT_PROFILE_SCOPE;
                FixedUpdate(PhysicsThreadFrameRate->DeltaTime);

                PhysicsThreadFrameRate->LockFrameRate();
//                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            ThreadCount--;

            MeowEngine::Log("Physics Thread", "Ended");
            WaitForThreadEndCondition.notify_all();
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

        // render ----------------
        std::thread RenderThread;
        // we decouple window / context into a class
        std::unique_ptr<MeowEngine::SDLWindow> WindowContext;
        std::shared_ptr<MeowEngine::graphics::ImGuiRenderer> UI;
        std::unique_ptr<MeowEngine::OpenGLRenderer> Renderer;
        std::unique_ptr<MeowEngine::graphics::OpenGLFrameBuffer> FrameBuffer;
        // this is shared because even main thread will access asset manager and sometimes physics
        // but render thread will access this all the time
        std::shared_ptr<MeowEngine::OpenGLAssetManager> AssetManager;


        void Render() {



            PT_PROFILE_SCOPE_N("setting current");
            // We let opengl know that any after this will be drawn into custom frame buffer
            {
                PT_PROFILE_SCOPE_N("framebuffer");
                FrameBuffer->Bind();

                {
                    PT_PROFILE_SCOPE_N("scene render");
                    glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    Scene->RenderGameView(*Renderer);

                    FrameBuffer->Unbind();
                }
            }

            glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            {
                PT_PROFILE_SCOPE_N("UI render");
//                MeowEngine::Log("Frame Rate: ", static_cast<int>(RenderThreadFrameRate.GetFrameRate()));
                Scene->RenderUserInterface(*Renderer, FrameBuffer->GetFrameTexture(), RenderThreadFrameRate->GetFrameRate());
            }

//                {
//                    PT_PROFILE_SCOPE_N("waiting");

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
//                }
//                {
//                    FpsCounter.frameEnd();
//                    PT_PROFILE_SCOPE_N("swap");
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
//                }
        };

        // physics thread ----------------

        void FixedUpdate(const float& inFixedDeltaTime) {
            Scene->AddEntitiesOnPhysicsThread(Physics.get());
            Physics->Update(inFixedDeltaTime);

            if(SyncPhysicMutex.try_lock()) {
                Scene->SyncPhysicsBufferOnPhysicsThread();
                SyncPhysicMutex.unlock();
            }
        };
    };
}


#endif //PHYSICAT_APPLICATION_TEST_HPP
