//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_SCENE_HPP
#define PHYSICAT_SCENE_HPP

#include "asset_manager.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"
#include "physics.hpp"

namespace physicat {
    struct Scene {
        Scene() = default;
        virtual ~Scene() = default;

        virtual void OnWindowResized(const physicat::WindowSize& size) = 0;

        virtual void Load(std::shared_ptr<physicat::AssetManager> assetManager) = 0;
        virtual void Create(std::shared_ptr<physicat::simulator::Physics> inPhysics) = 0; // Load assets
        virtual void Input(const float& deltaTime, const physicat::input::InputManager& inputManager) = 0;
        virtual void FixedUpdate(const float& inFixedDeltaTime, physicat::simulator::Physics& inPhysics) = 0;
        virtual void Update(const float& deltaTime) = 0;
        virtual void Render(physicat::Renderer& renderer) = 0;

        // Currently returns delta time
        // TODO: This will be converted into time manager which can contain more data.
        virtual const float& GetDeltaTime() = 0;
        virtual entt::registry* GetEntities() = 0;
    };
}


#endif //PHYSICAT_SCENE_HPP
