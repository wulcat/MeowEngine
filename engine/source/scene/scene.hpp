////
//// Created by Akira Mujawar on 21/06/24.
////
//
//#ifndef MEOWENGINE_SCENE_HPP
//#define MEOWENGINE_SCENE_HPP
//
//#include "asset_manager.hpp"
//#include "input_manager.hpp"
//#include "render_system.hpp"
//#include "physics_system.hpp"
//
//namespace MeowEngine {
//    // lets not have scene base class and just have single & multi-threaded scene
//    struct Scene {
//        Scene() = default;
//        virtual ~Scene() = default;
//
//        virtual void OnWindowResized(const MeowEngine::WindowSize& size) = 0;
//
//        // -----------------------------------------------------------------------
//
//        virtual void LoadOnRenderSystem(std::shared_ptr<MeowEngine::AssetManager> assetManager) = 0;
//
//        /**
//         * Creates scenes, adds entity / components
//         * (essentially later this might be a done dynamically by reading a saved scene data)
//         */
//        virtual void CreateSceneOnMainSystem() = 0;
//
//        /**
//         * When a entity is created on main thread, out buffer queues the entity to be created on physics thread
//         * and adds to staging(physics) buffer
//         * @param inPhysics
//         */
//        virtual void AddEntitiesOnPhysicsSystem(MeowEngine::simulator::PhysicsSystem* inPhysics) = 0;
//
//        // -----------------------------
//
//        virtual void Input(const float& deltaTime, const MeowEngine::input::InputManager& inputManager) = 0;
//
//        // -----------------------------
//
//        virtual void Update(const float& deltaTime) = 0;
//
//        // -----------------------------
//
//        virtual void RenderGameView(MeowEngine::RenderSystem& renderer) = 0;
//        virtual void RenderUserInterface(MeowEngine::RenderSystem& renderer, unsigned int frameBufferId, const double fps) = 0;
//
//        // -----------------------------
//
//        /**
//         * Based on availability of staging(physics) buffer  we either
//         * cache or sync data from main & render to physics & vice versa
//         * i.e. we take the updates from physics thread and push the updates from main thread w.r.t render thread
//         * @param inIsPhysicsThreadWorking
//         */
//        virtual void SyncPhysicsBufferOnMainSystem(bool inIsPhysicsThreadWorking) = 0;
//
//        /**
//         * Push rigidbody updates to transform & apply UI inputs to staging(physics) buffer
//         */
//        virtual void SyncPhysicsBufferOnPhysicsSystem() = 0;
//
//        // -----------------------------
//    };
//}
//
//
//#endif //MEOWENGINE_SCENE_HPP
