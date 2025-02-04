//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef MEOWENGINE_RENDER_SYSTEM_HPP
#define MEOWENGINE_RENDER_SYSTEM_HPP

#pragma once

#include "entt_wrapper.hpp"
#include "perspective_camera.hpp"
#include "reflection_property_change.hpp"
#include "queue"

namespace MeowEngine {
    struct RenderSystem {
        virtual void RenderGameView(MeowEngine::PerspectiveCamera* cameraObject, entt::registry& registry) = 0;
        virtual void RenderUserInterface(entt::registry& registry, std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& inUIInputQueue, unsigned int frameBufferId, const double fps) = 0;

        virtual void RenderPhysics(MeowEngine::PerspectiveCamera* cameraObject, entt::registry& registry) = 0;
    };
}


#endif //MEOWENGINE_RENDER_SYSTEM_HPP
