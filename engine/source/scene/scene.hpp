//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_SCENE_HPP
#define PHYSICAT_SCENE_HPP

#include "asset_manager.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"
#include "physics.hpp"

namespace MeowEngine {
    struct Scene {
        Scene() = default;
        virtual ~Scene() = default;

        virtual void OnWindowResized(const MeowEngine::WindowSize& size) = 0;

        virtual void Load(std::shared_ptr<MeowEngine::AssetManager> assetManager) = 0;
        virtual void Create() = 0; // Load assets
        virtual void CreatePhysics(MeowEngine::simulator::Physics* inPhysics) = 0;
        virtual void Input(const float& deltaTime, const MeowEngine::input::InputManager& inputManager) = 0;
        virtual void FixedUpdate(const float& inFixedDeltaTime) = 0;
        virtual void Update(const float& deltaTime) = 0;
        virtual void Render(MeowEngine::Renderer& renderer) = 0;
        virtual void RenderUI(MeowEngine::Renderer& renderer, unsigned int frameBufferId, const double fps) = 0;
        virtual void SwapBuffer() = 0;
        virtual void CalculateDeltaData() = 0;
        virtual void SyncPhysicsThreadData() = 0;
        virtual void SyncThreadData() = 0;

        // Currently returns delta time
        // TODO: This will be converted into time manager which can contain more data.
        virtual const float& GetDeltaTime() = 0;
    };
}


#endif //PHYSICAT_SCENE_HPP
