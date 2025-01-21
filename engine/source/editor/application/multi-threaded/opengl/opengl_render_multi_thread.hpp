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

using namespace std;

namespace MeowEngine {
    class ApplicationTest;

    class OpenGLRenderThread {
    public:
        OpenGLRenderThread();
        ~OpenGLRenderThread();

        void StartThread(MeowEngine::ApplicationTest& inApplication);
        void EndThread();

        void RenderThreadLoop(ApplicationTest& inApplication);
        void Render(ApplicationTest& inApplication);

        std::unique_ptr<FrameRateCounter> RenderThreadFrameRate;
        std::thread RenderThread;
        // we decouple window / context into a class
        std::unique_ptr<MeowEngine::SDLWindow> WindowContext;
        std::shared_ptr<MeowEngine::graphics::ImGuiRenderer> UI;
        std::unique_ptr<MeowEngine::OpenGLRenderer> Renderer;
        std::unique_ptr<MeowEngine::graphics::OpenGLFrameBuffer> FrameBuffer;
        // this is shared because even main thread will access asset manager and sometimes physics
        // but render thread will access this all the time
        std::shared_ptr<MeowEngine::OpenGLAssetManager> AssetManager;
    };

} // MeowEngine

#endif //MEOWENGINE_OPENGL_RENDER_MULTI_THREAD_HPP
