//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_SCENE_HPP
#define PHYSICAT_SCENE_HPP

#include "asset_manager.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"

namespace physicat {
    struct Scene {
        Scene() = default;
        virtual ~Scene() = default;

        virtual void OnWindowResized(const physicat::WindowSize& size) = 0;

        virtual void Create(physicat::AssetManager& assetManager) = 0; // Load assets
        virtual void Input(const float& deltaTime, const physicat::input::InputManager& inputManager) = 0;
        virtual void Update(const float& deltaTime) = 0;
        virtual void Render(physicat::Renderer& renderer) = 0;
    };
}


#endif //PHYSICAT_SCENE_HPP
