//
// Created by Akira Mujawar on 26/01/25.
//

#ifndef MEOWENGINE_OPENGL_APP_WEB_HPP
#define MEOWENGINE_OPENGL_APP_WEB_HPP

#include "application.hpp"

#include "memory"

#include "frame_rate_counter.hpp"
#include "input_manager.hpp"
//#include "scene_single_thread.hpp"
#include "sdl_window.hpp"
#include "imgui_user_interface_system.hpp"
#include "opengl_render_system.hpp"
#include "physx_physics_system.hpp"

#include "emscripten_wrapper.hpp"

#include "scene_multi_thread.hpp"

using namespace std;

namespace MeowEngine {

    struct OpenGLAppWeb : public MeowEngine::Application {
    public:
        OpenGLAppWeb() {
            MeowEngine::Log("Application", "Created");
        }
        ~OpenGLAppWeb() {
            MeowEngine::Log("Application", "Destroyed");
        }
        // main
        void CreateApplication() override;

        void CreateScene();
        void EngineLoop();
        bool ProcessDeviceEvents(const float& inDeltaTime);

        // render
        void RenderGameView();
        void RenderUserInterface();

        // main
        std::unique_ptr<MeowEngine::FrameRateCounter> FrameRateCounter;
        std::unique_ptr<MeowEngine::input::InputManager> InputManager;

        // shared
        std::shared_ptr<MeowEngine::SceneMultiThread> Scene;

        // render
        std::unique_ptr<MeowEngine::SDLWindow> WindowContext;
        std::shared_ptr<MeowEngine::graphics::ImGuiUserInterfaceSystem> UserInterface;
        std::unique_ptr<MeowEngine::OpenGLRenderSystem> GameView;

        /**
         * Draws our game view onto a ui panel (which is a framebuffer)
         */
        std::unique_ptr<MeowEngine::graphics::OpenGLFrameBuffer> FrameBuffer;

        /**
         * this is shared because even main thread will access asset manager and sometimes physics
         * but render thread will access this all the time
         */
        std::shared_ptr<MeowEngine::OpenGLAssetManager> AssetManager;

        // physics
        std::shared_ptr<MeowEngine::simulator::PhysicsSystem> Physics;

    };

} // MeowEngine

#endif //MEOWENGINE_OPENGL_APP_WEB_HPP
