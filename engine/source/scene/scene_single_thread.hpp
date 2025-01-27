//
// Created by Akira Mujawar on 27/01/25.
//

#ifndef MEOWENGINE_SCENE_SINGLE_THREAD_HPP
#define MEOWENGINE_SCENE_SINGLE_THREAD_HPP

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
        void Load(std::shared_ptr<MeowEngine::AssetManager> assetManager);
        void Create();
        void CreateEntitiesForPhysics(MeowEngine::simulator::PhysicsSystem* inPhysics); // we will use queue for this as well similar to multi threading
        void Input(const float& deltaTime, const MeowEngine::input::InputManager& inputManager);
        void Update(const float& deltaTime);
        void RenderGameView(MeowEngine::RenderSystem& renderer);
        void RenderUserInterface(MeowEngine::RenderSystem& renderer, unsigned int frameBufferId, const double fps);

        /**
        * Sync updates from rigidbody to transform component & apply UI inputs to staging(physics) buffer
        */
        void SyncMain();

        /**
         * Sync updates from transform to rigidbody component
         */
        void SyncPhysics();

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };

} // MeowEngine

#endif //MEOWENGINE_SCENE_SINGLE_THREAD_HPP
