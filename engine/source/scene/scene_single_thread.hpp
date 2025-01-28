//
// Created by Akira Mujawar on 27/01/25.
//

#ifndef MEOWENGINE_SCENE_SINGLE_THREAD_HPP
#define MEOWENGINE_SCENE_SINGLE_THREAD_HPP

#include "scene.hpp"
#include "asset_manager.hpp"
#include "input_manager.hpp"
#include "render_system.hpp"
#include "physics_system.hpp"

// main update - creates something for both physics and main
// apply
// physics update
// apply
// render

namespace MeowEngine {

    struct SceneSingleThread {
        SceneSingleThread(const MeowEngine::WindowSize&);

        void OnWindowResized(const MeowEngine::WindowSize& size);
        void LoadOnRenderSystem(std::shared_ptr<MeowEngine::AssetManager> assetManager);
        void CreateSceneOnMainSystem(MeowEngine::simulator::PhysicsSystem* inPhysics);

        void Input(const float& deltaTime, const MeowEngine::input::InputManager& inputManager);
        void Update(const float& deltaTime);
        void RenderGameView(MeowEngine::RenderSystem& renderer);
        void RenderUserInterface(MeowEngine::RenderSystem& renderer, unsigned int frameBufferId, const double fps);

        /**
        * Sync updates from rigidbody to transform component & apply UI inputs to staging(physics) buffer
        */
        void ApplyUpdateChanges();

        /**
         * Sync updates from transform to rigidbody component
         */
        void ApplyPhysicSystemChanges();

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };

} // MeowEngine

#endif //MEOWENGINE_SCENE_SINGLE_THREAD_HPP
