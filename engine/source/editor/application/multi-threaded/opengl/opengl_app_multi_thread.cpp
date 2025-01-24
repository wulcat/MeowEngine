//
// Created by Akira Mujawar on 21/01/25.
//

#include "opengl_app_multi_thread.hpp"

#include "main_scene.hpp"

namespace MeowEngine {
    OpenGLAppMultiThread::OpenGLAppMultiThread()
        : SharedState() {
        MeowEngine::Log("OpenGL", "App Created");
    }

//    OpenGLRenderMultiThread::~OpenGLRenderMultiThread() {
//        MeowEngine::Log("OpenGL", "App Destroyed");
//    }

    void OpenGLAppMultiThread::CreateApplication() {
        // Initialize Shared State
        SharedState.IsAppRunning = true;
        SharedState.SyncPointStartRenderBarrier = std::make_shared<ThreadBarrier>(2);
        SharedState.SyncPointEndRenderBarrier = std::make_shared<ThreadBarrier>(2);

#ifdef __EMSCRIPTEN__
        //  emscripten_set_main_loop(emscriptenLoop, 60, 1);
                emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
#else

         // Create Thread Wrappers with their sub-systems
        RenderThread = std::make_unique<MeowEngine::OpenGLRenderMultiThread>(SharedState);
        PhysicThread = std::make_unique<MeowEngine::PhysicsMultiThread>(SharedState);

        // Create Scene & Setup Main thread
        FrameRateCounter = std::make_unique<MeowEngine::FrameRateCounter>(60, 1); // 60 frames per second
        InputManager = std::make_unique<MeowEngine::input::InputManager>();
        Scene = std::make_shared<MeowEngine::MainScene>(
            MeowEngine::sdl::GetWindowSize(RenderThread->WindowContext->window)
        );

        // Update Thread Wrappers to utilize active scene
        RenderThread->SetScene(Scene);
        PhysicThread->SetScene(Scene);

        // Start Rendering & Physics Simulation
        RenderThread->StartThread();
        PhysicThread->StartThread();

        // Start updating game
        // Method is executed until, game loop is active.
        // Once ended we process with closing engine
        EngineLoop();

        // Stop threads
        RenderThread->EndThread();
        PhysicThread->EndThread();

        // Remove systems
        InputManager.reset();

        MeowEngine::Log("Application", "Ended");
#endif
    }

    void OpenGLAppMultiThread::EngineLoop() {
        PT_PROFILE_SCOPE;
        MeowEngine::Log("Main Thread", "Started");

        // TODO: We do this in later days, when we want to load scene dynamically
        // load resources
        // create scene on main thread
        // create physics objects
        // then start

        // Create scene objects like cubes, grid and etc...
        Scene->CreateSceneOnMainThread();

        // Purposefully waiting so the render thread loads required shaders & objs
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Simulating Scene
        while (SharedState.IsAppRunning) {
            // Calculate Delta Time
            FrameRateCounter->Calculate();

            // Update Scene
            Scene->Update(FrameRateCounter->DeltaTime);

            // Check for device inputs
            if (!ProcessDeviceEvents(FrameRateCounter->DeltaTime)) {
                InitiateAppClose();

                break;
            }

            SyncThreads();

            // Halt Main thread to lock a frame rate
            FrameRateCounter->LockFrameRate();

            // Wait until buffers are synced
            // Ideally since other threads are waiting this should release all of them automatically
            SharedState.SyncPointEndRenderBarrier->Wait();
        }

        MeowEngine::Log("Main Thread", "Waiting for other threads to end");

        // Wait till all threads are closed
        SharedState.ActiveWaitThread.Wait([&](int inActiveThreads){
            return inActiveThreads == 0;
        });

        MeowEngine::Log("Main Thread", "Ended");
    }

    bool OpenGLAppMultiThread::ProcessDeviceEvents(const float &deltaTime) {
        PT_PROFILE_SCOPE;

//            while(!InputBuffer.GetCurrent().empty())
//            {
//                event = InputBuffer.GetCurrent().front();
//                InputBuffer.GetCurrent().pop();
//
//                // perform the task
//                MeowEngine::Log("InputBuffer", "Event");
//            }

        // Each loop we will process any events that are waiting for us.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            SharedState.SDLEventBuffer.GetCurrent().push(event);

            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    InputManager->SetMouseDown();
                    break;

                case SDL_QUIT:
                    // If we get a quit signal, we will return that we don't want to keep looping.
                    RenderThread->UserInterface->ClosePIDs();
                    return false;

                case SDL_KEYDOWN:
                    // If we get a key down event for the ESC key, we also don't want to keep looping.
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        RenderThread->UserInterface->ClosePIDs();

                        // App should close
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

        // TODO: Build a input system
        // Track keyboard and mouse clicks/hold/drag/position
        InputManager->ProcessInput();
        Scene->Input(deltaTime, *InputManager);
        InputManager->isMouseDown = false;

        // should app continue?
        return true;
    }

    void OpenGLAppMultiThread::SyncThreads() {
        // Make render thread wait, until syncing is complete
        SharedState.SyncPointStartRenderBarrier->Wait();

        // Swap Device Events
        SharedState.SDLEventBuffer.Swap();

        // Sync current(main) buffer rigid bodies
        // & update staging(physics) buffer with current buffer updates
        if (SharedState.SyncPointPhysicMutex.try_lock()) {
            // Sync staging buffer and apply current buffer updates
            Scene->SyncPhysicsBufferOnMainThread(false);
            SharedState.SyncPointPhysicMutex.unlock();
        } else {
            // Cache current buffer updates
            // (which are applied when staging buffer is free)
            Scene->SyncPhysicsBufferOnMainThread(true);
        }


        Scene->SyncRenderBufferOnMainThread();
        Scene->SwapMainAndRenderBufferOnMainThread();
    }

    void OpenGLAppMultiThread::InitiateAppClose() {
        // Initiate closing engine
        SharedState.IsAppRunning = false;

        // if threads are waiting for thread sync we unpause them
        SharedState.SyncPointStartRenderBarrier->End();
        SharedState.SyncPointEndRenderBarrier->End();
        MeowEngine::Log("Main Thread", "Ending");
    }

}



// - create renderer (game & ui)
// - create physics simulator
// - create input manager
// - create scene

// - load assets
// - create scene data (uses loaded assets)
// - create physics data (uses scene data & loaded assets)

// -------------------- thread updates start

// loop physics thread
// - update
// - process request for creating life object

// loop render thread
// - process input requests from buffer
// - update game using final buffer
// - update ui using final buffer (queue for any ui inputs)
// - swap (internal render buffer)

// loop main thread
// - game update
// - input
//   - device input
//   - process input manager
//   - scene input
// - swap input buffer
// - sync thread data
//   - apply current buffer to final buffer
//   - apply ui changes from queue to current & final buffer (physics in tbd)
//   - add life objects as per requests - (can be from ui inputs / game update)
//   - queue request for physics to added life objects
// - swap game buffer

// -------------------- thread updates end

// - pause rendering
// - pause physics simulator updates
// - pause scene updates

// - destroy physics simulator
// - destroy scene data
// - destroy loaded assets


// ------------------
// main thread

// create scene data
// starts looping
// ends

// ------------------
// render thread

// load assets
// starts looping
// ends

// ------------------
// physics thread

// create physics data
// starts looping
// ends
