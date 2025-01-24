//
// Created by Akira Mujawar on 20/01/25.
//

#include "opengl_render_multi_thread.hpp"

#include "shared_thread_state.hpp"

namespace MeowEngine {
    OpenGLRenderMultiThread::OpenGLRenderMultiThread(MeowEngine::SharedThreadState& inState)
    : SharedState(inState) {
        MeowEngine::Log("Render", "Creating Object");

        WindowContext = std::make_unique<MeowEngine::SDLWindow>();
        AssetManager = std::make_shared<MeowEngine::OpenGLAssetManager>(MeowEngine::OpenGLAssetManager());
        UserInterface = std::make_shared<MeowEngine::graphics::ImGuiUserInterfaceSystem>(WindowContext->window, WindowContext->context);
        GameView = std::make_unique<MeowEngine::OpenGLRenderSystem>(AssetManager, UserInterface);
        FrameBuffer = std::make_unique<MeowEngine::graphics::OpenGLFrameBuffer>(1000,500);
        FrameRateCounter = std::make_unique<MeowEngine::FrameRateCounter>(60, 100);
    }

    OpenGLRenderMultiThread::~OpenGLRenderMultiThread() {
        FrameRateCounter.reset();
        AssetManager.reset();
        FrameBuffer.reset();
        UserInterface.reset();
        GameView.reset();
        WindowContext.reset();
    }

    void OpenGLRenderMultiThread::SetScene(std::shared_ptr<MeowEngine::Scene> inScene) {
        Scene = inScene;
    }

    void OpenGLRenderMultiThread::StartThread() {
        MeowEngine::Log("Render", "Starting");

        // NOTE: Clearing context in main thread before using for render thread fixes a crash
        // which occurs while drag window
        SDL_GL_MakeCurrent(WindowContext->window, nullptr);

        RenderThread = std::thread(&MeowEngine::OpenGLRenderMultiThread::RenderThreadLoop, this);
    }

    void OpenGLRenderMultiThread::EndThread() {
        RenderThread.join();
    }

    void OpenGLRenderMultiThread::RenderThreadLoop() {
        MeowEngine::Log("Render Thread", "Started");

        // Count active threads so we can close engine properly
        SharedState.ActiveWaitThread.GetAtomic()++;

        // Switch SDL to use render thread
        SDL_GL_MakeCurrent(WindowContext->window, WindowContext->context);

        // Load Scene Resources like shaders and meshes
        Scene->LoadOnRenderThread(AssetManager);

        // Render Graphics
        while (SharedState.IsAppRunning) {
            PT_PROFILE_SCOPE;

            // Calculate render frame rate
            FrameRateCounter->Calculate();

            // Process any device events like resize
            ProcessDeviceEvents(SharedState.SDLEventBuffer.GetFinal());

            // Render our game view
            RenderGameView();
            RenderUserInterface();

            WindowContext->SwapWindow();

            // MeowEngine::Log("Render Thread", "Waiting for other threads to finish processing");
            // wait for all threads to sync up for frame ending
            SharedState.SyncPointStartRenderBarrier->Wait();

            // MeowEngine::Log("Render Thread", "Waiting for main thread to finish swapping buffers");
            // wait until buffers are synced on main thread
            SharedState.SyncPointEndRenderBarrier->Wait();
        }

        // Just in case extra delay before closing
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Notify main thread, that thread count has changed
        SharedState.ActiveWaitThread.GetAtomicAndNotify().Get()--;

        MeowEngine::Log("Render Thread", "Ended");
    }

    void OpenGLRenderMultiThread::ProcessDeviceEvents(std::queue<SDL_Event>& inEvents) {
        PT_PROFILE_SCOPE;

        while(!inEvents.empty()) {
            SDL_Event event = inEvents.front();
            inEvents.pop();

            UserInterface->Input(event);

            switch (event.type) {
                case SDL_USEREVENT:
                    switch (event.user.code) {
                        case 2: {
                            const WindowSize size = *(WindowSize *) event.user.data1;

                            glViewport(0, 0, size.Width, size.Height);
                            FrameBuffer->RescaleFrameBuffer(size.Width, size.Height);
                            MeowEngine::Log("Render Thread", "Rescaled");
                            break;
                        }
                    }
            }
        }
    }

    void OpenGLRenderMultiThread::RenderGameView() {
        PT_PROFILE_SCOPE;

        // We let opengl know that any after this will be drawn into custom frame buffer
        FrameBuffer->Bind();

        glClearColor(38 / 255.0f, 38 / 255.0f, 38 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene->RenderGameView(*GameView);

        // Let opengl know again, that we don't have to draw on custom frame buffer
        FrameBuffer->Unbind();
    }

    void OpenGLRenderMultiThread::RenderUserInterface() {
        PT_PROFILE_SCOPE;

        glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene->RenderUserInterface(*GameView, FrameBuffer->GetFrameTexture(), FrameRateCounter->GetFrameRate());
    }

} // MeowEngine



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