//
// Created by Akira Mujawar on 20/01/25.
//

#include "opengl_render_multi_thread.hpp"
#include "shared_thread_state.hpp"
#include "sdl_wrapper.hpp"

namespace MeowEngine {
    OpenGLRenderMultiThread::OpenGLRenderMultiThread(MeowEngine::SharedThreadState& inState)
    : SharedState(inState) {
        MeowEngine::Log("Render", "Creating Object");



        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
            throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
        }

        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            throw std::runtime_error("Main Thread:: Could not initialize SDL2_image");
        }

        MeowEngine::Log("Main Thread", "SDL2 Initialized");

        WindowContext = std::make_unique<MeowEngine::SDLWindow>();
        AssetManager = std::make_shared<MeowEngine::OpenGLAssetManager>(MeowEngine::OpenGLAssetManager());
        UI = std::make_shared<MeowEngine::graphics::ImGuiRenderer>(WindowContext->window, WindowContext->context);
        Renderer = std::make_unique<MeowEngine::OpenGLRenderSystem>(AssetManager, UI);
        FrameBuffer = std::make_unique<MeowEngine::graphics::OpenGLFrameBuffer>(1000,500);
        RenderThreadFrameRate = std::make_unique<FrameRateCounter>(60, 100);
    }

    OpenGLRenderMultiThread::~OpenGLRenderMultiThread() {
        AssetManager.reset();
        FrameBuffer.reset();
        UI.reset();
        Renderer.reset();
        WindowContext.reset();
    }

    void OpenGLRenderMultiThread::SetScene(std::shared_ptr<MeowEngine::Scene> inScene) {
        Scene = inScene;
    }

    void OpenGLRenderMultiThread::StartThread() {
        MeowEngine::Log("Render", "Starting Thread");

        // NOTE: Clearing context in main thread before using for render thread fixes a crash
        // which occurs while drag window
        SDL_GL_MakeCurrent(WindowContext->window, nullptr);

        RenderThread = std::thread(&MeowEngine::OpenGLRenderMultiThread::RenderThreadLoop, this);
    }
    void OpenGLRenderMultiThread::EndThread() {
        RenderThread.join();
    }

    void OpenGLRenderMultiThread::RenderThreadLoop() {

        // init
        MeowEngine::Log("Render Thread", "Started");
        SharedState.ActiveWaitThread.GetAtomic()++;

        SDL_GL_MakeCurrent(WindowContext->window, WindowContext->context);
        Scene->LoadOnRenderThread(AssetManager);

        // loop
        while (SharedState.IsAppRunning) {
//                Uint64 currentTime = SDL_GetPerformanceCounter();
            RenderThreadFrameRate->Calculate();

            PT_PROFILE_SCOPE;
            // Synchronize with the main thread
            // input
            while(!SharedState.SDLEventBuffer.GetFinal().empty()) {
                SDL_Event event = SharedState.SDLEventBuffer.GetFinal().front();
                SharedState.SDLEventBuffer.GetFinal().pop();

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
            SharedState.SyncPointStartRenderBarrier.get()->Wait();

            // MeowEngine::Log("Render Thread", "Waiting for main thread to finish swapping buffers");
            // wait until buffers are synced on main thread
            SharedState.SyncPointEndRenderBarrier.get()->Wait();


//                RenderThreadFrameRate.End();
        }

        // exit
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        SharedState.ActiveWaitThread.GetAtomicAndNotify().Get()--;

        MeowEngine::Log("Render Thread", "Ended");

    }

    void OpenGLRenderMultiThread::Render() {
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
    }


} // MeowEngine