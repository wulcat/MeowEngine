//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_RENDERER_HPP
#define PHYSICAT_RENDERER_HPP

#pragma once

#include "entt_wrapper.hpp"
#include "perspective_camera.hpp"
#include "reflection_property_change.hpp"
#include "queue"

namespace physicat {
    struct Renderer {
        virtual void Render(physicat::PerspectiveCamera* cameraObject, entt::registry& registry) = 0;
        virtual void RenderUI(entt::registry& registry, std::queue<std::shared_ptr<physicat::ReflectionPropertyChange>>& inUIInputQueue, unsigned int frameBufferId, const double fps) = 0;
    };
}


#endif //PHYSICAT_RENDERER_HPP
