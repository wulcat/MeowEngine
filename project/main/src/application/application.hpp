//
// Created by Akira Lynn on 06/07/22.
//

#ifndef VULKAN_ENGINE_ENGINE_HPP
#define VULKAN_ENGINE_ENGINE_HPP

#pragma once

#include "../core/includes.hpp"
#include "../simulations/scene.hpp"

namespace physicat {
    struct Application {
        Application() = default;
        virtual ~Application() = default;

        void Begin();
        bool Update();
        virtual void Render(Scene* scene) = 0;

        // TEMP -- start

        Scene* scene{};

        // TEMP -- end
    };
} // namespace physicat


#endif //VULKAN_ENGINE_ENGINE_HPP
