//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef VULKAN_ENGINE_ENGINE_HPP
#define VULKAN_ENGINE_ENGINE_HPP

#pragma once

//#include <frame_rate_counter.hpp>
//#include "internal_ptr.hpp"
//#include "emscripten_wrapper.hpp"
//
//#include "thread"
//#include "log.hpp" // temp
//#include "entt_reflection.hpp"

using namespace std;

namespace MeowEngine {
    struct Application {
        Application() = default;
        virtual ~Application() = default;

        virtual void CreateApplication() = 0;
//        bool LoopApplication();

//        virtual bool Input(const float& deltaTime) = 0;
//        virtual void FixedUpdate(const float& inFixedDeltaTime) = 0;
//        virtual void Update(const float& deltaTime) = 0;
//        virtual void Render() = 0;

//    private:
//        struct Internal;
//        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
} // namespace MeowEngine


#endif //VULKAN_ENGINE_ENGINE_HPP
