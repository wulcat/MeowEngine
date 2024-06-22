//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_MAIN_SCENE_HPP
#define PHYSICAT_MAIN_SCENE_HPP

#include "internal_ptr.hpp"
#include "scene.hpp"

namespace physicat {
    struct MainScene : public physicat::Scene {
        MainScene(const float& screenWidth, const float& screenHeight);

        void Create(physicat::AssetManager& assetManager) override;
        void Update(const float& deltaTime) override;
        void Render(physicat::Renderer& renderer) override;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_MAIN_SCENE_HPP
