//
// Created by Akira Mujawar on 21/01/25.
//

#ifndef MEOWENGINE_OPENGL_APP_MULTI_THREAD_HPP
#define MEOWENGINE_OPENGL_APP_MULTI_THREAD_HPP

#include "input_manager.hpp"
#include "frame_rate_counter.hpp"
#include "shared_thread_state.hpp"

#include "physics_multi_thread.hpp"
#include "opengl_render_multi_thread.hpp"

#include "scene_multi_thread.hpp"

using namespace std;

namespace MeowEngine {
    class OpenGLAppMultiThread {
    public:
        OpenGLAppMultiThread();
//        ~OpenGLAppMultiThread();

        void CreateApplication();

    private:
        void EngineLoop();

        bool ProcessDeviceEvents(const float &deltaTime);

        /**
         * Sync Buffers from different threads
         */
        void SyncThreads();

        /**
         * We tell others threads to end so we can close the engine
         */
        void InitiateAppClose();

        /**
         * Shared states between threads to manage syncing and engine life cycle
         */
        MeowEngine::SharedThreadState SharedState;

        /**
         * Detects the frame rate for delta time calculation
         */
        std::unique_ptr<MeowEngine::FrameRateCounter> FrameRateCounter;

        /**
         * Manages Device Inputs
         */
        std::unique_ptr<MeowEngine::input::InputManager> InputManager;

        /**
         * Active Scene in engine
         * (once we have more than one scene we will think to how to load dynamically)
         */
        std::shared_ptr<MeowEngine::SceneMultiThread> Scene;

        /**
         * Physics Wrapper for simulating in different thread
         */
        std::unique_ptr<MeowEngine::PhysicsMultiThread> PhysicThread;

        /**
         * Graphics Wrapper for simulating in different thread (will come back)
         */
        std::unique_ptr<MeowEngine::OpenGLRenderMultiThread> RenderThread;
    };
}

#endif //MEOWENGINE_OPENGL_APP_MULTI_THREAD_HPP