//
// Created by Akira Mujawar on 20/01/25.
//

#ifndef MEOWENGINE_OPENGL_RENDER_MULTI_THREAD_HPP
#define MEOWENGINE_OPENGL_RENDER_MULTI_THREAD_HPP

#include "frame_rate_counter.hpp"
#include "thread"
//#include "tracy_wrapper.hpp"
#include "graphics_wrapper.hpp"
//#include "sdl_wrapper.hpp"
#include "sdl_window.hpp"
#include "asset_manager.hpp"
#include "opengl_render_system.hpp"
#include "scene.hpp"

using namespace std;

namespace MeowEngine {
    class SharedThreadState;

    class OpenGLRenderMultiThread {
    public:
        OpenGLRenderMultiThread(MeowEngine::SharedThreadState& inState);
        ~OpenGLRenderMultiThread();

        void SetScene(std::shared_ptr<MeowEngine::Scene> inScene);
        void StartThread();
        void EndThread();

        void RenderThreadLoop();
        void Render();

        std::unique_ptr<FrameRateCounter> RenderThreadFrameRate;
        std::thread RenderThread;
        // we decouple window / context into a class
        std::unique_ptr<MeowEngine::SDLWindow> WindowContext;
        std::shared_ptr<MeowEngine::graphics::ImGuiRenderer> UI;
        std::unique_ptr<MeowEngine::OpenGLRenderSystem> Renderer;
        std::unique_ptr<MeowEngine::graphics::OpenGLFrameBuffer> FrameBuffer;
        // this is shared because even main thread will access asset manager and sometimes physics
        // but render thread will access this all the time
        std::shared_ptr<MeowEngine::OpenGLAssetManager> AssetManager;

        MeowEngine::SharedThreadState& SharedState;
        std::shared_ptr<MeowEngine::Scene> Scene;
    };

} // MeowEngine

#endif //MEOWENGINE_OPENGL_RENDER_MULTI_THREAD_HPP
