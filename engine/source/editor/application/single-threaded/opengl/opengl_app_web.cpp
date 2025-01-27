//
// Created by Akira Mujawar on 26/01/25.
//

#include "opengl_app_web.hpp"

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

        WindowContext = std::make_unique<MeowEngine::SDLWindow>();
        AssetManager = std::make_shared<MeowEngine::OpenGLAssetManager>(MeowEngine::OpenGLAssetManager());

        FrameBuffer = std::make_unique<MeowEngine::graphics::OpenGLFrameBuffer>(1000,500);
        FrameRateCounter = std::make_unique<MeowEngine::FrameRateCounter>(60, 100);

        UserInterface = std::make_shared<MeowEngine::graphics::ImGuiUserInterfaceSystem>(WindowContext->window, WindowContext->context);
        GameView = std::make_unique<MeowEngine::OpenGLRenderSystem>(AssetManager, UserInterface);
        Physics = std::make_shared<MeowEngine::simulator::PhysXPhysicsSystem>();

        MeowEngine::Log("Application", "Created Required Systems");
#ifdef EMSCRIPTEN
        // Free Function in an anonymous namespace private to this file
//                engine->LoopApplication();
//                    void EmscriptenLoop(MeowEngine::Application* engine) {
//                        engine->LoopApplication();
//                    }
                emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
#endif

        FrameRateCounter.reset();
        AssetManager.reset();
        FrameBuffer.reset();

        WindowContext.reset();

        UserInterface.reset();
        GameView.reset();
        Physics.reset();
    }
} // MeowEngine