//
// Created by Akira Lynn on 06/07/22.
//

#ifndef VULKAN_ENGINE_ENGINE_HPP
#define VULKAN_ENGINE_ENGINE_HPP

#pragma once

#include "internal_ptr.hpp"
#include "includes.hpp"

namespace physicat {
    struct Application {
        Application();
        virtual ~Application() = default;

        void StartApplication();
        bool LoopApplication();
//        virtual void OnWindowResized() = 0;

        virtual bool Input(const float& deltaTime) = 0;
        virtual void Update(const float& deltaTime) = 0;
        virtual void Render() = 0;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat


#endif //VULKAN_ENGINE_ENGINE_HPP
