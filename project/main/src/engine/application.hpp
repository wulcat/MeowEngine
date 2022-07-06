//
// Created by Akira Lynn on 06/07/22.
//

#ifndef VULKAN_ENGINE_ENGINE_HPP
#define VULKAN_ENGINE_ENGINE_HPP

#pragma once

#include "../core/includes.hpp"

namespace physicat {
    struct Application {
        Application() = default;
        virtual ~Application() = default;

        void StartApplication();
        bool Update();
        virtual void Render() = 0;
    };
} // namespace physicat


#endif //VULKAN_ENGINE_ENGINE_HPP
