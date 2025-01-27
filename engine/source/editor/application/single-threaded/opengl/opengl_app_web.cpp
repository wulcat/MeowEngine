//
// Created by Akira Mujawar on 26/01/25.
//

#include "opengl_app_web.hpp"

#include "scene_single_thread.hpp"

// creation management
// input management
// scene management
// loading management

namespace {
#ifdef EMSCRIPTEN
    // Free Function in an anonymous namespace private to this file
        void EmscriptenLoop(MeowEngine::OpenGLAppWeb* engine) {
            MeowEngine::Log("Emscripten", "Looping");
            engine->EngineLoop();
        }
#endif
} // namespace
namespace MeowEngine {
    void OpenGLAppWeb::CreateApplication() {
        MeowEngine::Log("Application", "Creating Object");

        FrameRateCounter = std::make_unique<MeowEngine::FrameRateCounter>(60, 100);
        InputManager = std::make_unique<MeowEngine::input::InputManager>();

        WindowContext = std::make_unique<MeowEngine::SDLWindow>();
        AssetManager = std::make_shared<MeowEngine::OpenGLAssetManager>(MeowEngine::OpenGLAssetManager());
        FrameBuffer = std::make_unique<MeowEngine::graphics::OpenGLFrameBuffer>(1000,500);

        UserInterface = std::make_shared<MeowEngine::graphics::ImGuiUserInterfaceSystem>(WindowContext->window, WindowContext->context);
        GameView = std::make_unique<MeowEngine::OpenGLRenderSystem>(AssetManager, UserInterface);
        Physics = std::make_shared<MeowEngine::simulator::PhysXPhysicsSystem>();


        MeowEngine::Log("Application", "Created Required Systems");

        CreateScene();

#ifdef __EMSCRIPTEN__
        // Free Function in an anonymous namespace private to this file
//                engine->LoopApplication();
//                    void EmscriptenLoop(MeowEngine::Application* engine) {
//                        engine->LoopApplication();
//                    }
                emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
#endif

//        FrameRateCounter.reset();
//        AssetManager.reset();
//        FrameBuffer.reset();
//
//        WindowContext.reset();
//
//        UserInterface.reset();
//        GameView.reset();
//        Physics.reset();
    }

    void OpenGLAppWeb::CreateScene() {
        Scene = std::make_shared<MeowEngine::SceneSingleThread>(
                MeowEngine::sdl::GetWindowSize(WindowContext->window)
        );

        Physics->Create();

        Scene->LoadOnRenderSystem(AssetManager);
        Scene->CreateSceneOnMainSystem();
    }

    void OpenGLAppWeb::EngineLoop() {
        FrameRateCounter->Calculate();

        ProcessDeviceEvents(FrameRateCounter->DeltaTime);

        Scene->Update(FrameRateCounter->DeltaTime);
        Scene->SyncPhysicsBufferOnMainSystem();

        Scene->AddEntitiesOnPhysicsSystem(Physics.get());
        Physics->Update(FrameRateCounter->DeltaTime);
        Scene->SyncPhysicsBufferOnPhysicsSystem();

        RenderGameView();
        RenderUserInterface();
        WindowContext->SwapWindow();
    }

    bool OpenGLAppWeb::ProcessDeviceEvents(const float& inDeltaTime) {
        SDL_Event event;

        // Each loop we will process any events that are waiting for us.
        while (SDL_PollEvent(&event))
        {
            UserInterface->Input(event);

            switch (event.type)
            {
                case SDL_WINDOWEVENT:
//                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
//                        OnWindowResized();
//                    }
                    break;

//                // If we get a quit signal, we will return that we don't want to keep looping.
//                case SDL_QUIT:
//                    UI.ClosePIDs();
//                    return false;

//                case SDL_KEYDOWN:
//                    // If we get a key down event for the ESC key, we also don't want to keep looping.
//                    if (event.key.keysym.sym == SDLK_ESCAPE) {
//                        UI.ClosePIDs();
//                        return false;
//                    }
//                    break;

                case SDL_USEREVENT:
                    switch (event.user.code) {
                        case 2: {
                            const WindowSize size = *(WindowSize *) event.user.data1;

                            glViewport(0, 0, size.Width, size.Height);
                            FrameBuffer->RescaleFrameBuffer(size.Width, size.Height);
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
        Scene->Input(inDeltaTime, *InputManager);
        InputManager->isMouseDown = false;

        return true;
    }

    void OpenGLAppWeb::RenderGameView() {
        // We let opengl know that any after this will be drawn into custom frame buffer
        FrameBuffer->Bind();

        glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene->RenderGameView(*GameView);

        // Let opengl know again, that we don't have to draw on custom frame buffer
        FrameBuffer->Unbind();
    }

    void OpenGLAppWeb::RenderUserInterface() {
        glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene->RenderUserInterface(*GameView, FrameBuffer->GetFrameTexture(), FrameRateCounter->GetFrameRate());
    }
} // MeowEngine