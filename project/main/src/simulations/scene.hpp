//
// Created by Akira Lynn on 07/07/22.
//

#ifndef PHYSICAT_SCENE_HPP
#define PHYSICAT_SCENE_HPP

#include "fluids/basic/fluid.hpp"
#include "SDL.h"
#include "../core/log.hpp"

namespace physicat {
    class Scene {
        Fluid *fluid{};

    public:
        Scene() = default;

        void Begin() {
            fluid = new Fluid(50);
        }

        void Update() {
//        physicat::log("Scene Update","update");
            fluid->Update();
        }

        void Render(SDL_Renderer *renderer) {
//        physicat::log("Scene Update","render");
            fluid->Render(renderer);
        }
    };
}
#endif //PHYSICAT_SCENE_HPP
