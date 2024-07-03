//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_RENDERER_HPP
#define PHYSICAT_RENDERER_HPP

#pragma once

//#include "asset_inventory.hpp"
//#include "static_mesh_instance.hpp"
//#include "opengl_line_pipeline.hpp"
//#include "render_component_base.hpp"
#include "life_object.hpp"
#include "perspective_camera.hpp"

//#include <vector>

namespace physicat {
    struct Renderer {
        virtual void Render(physicat::PerspectiveCamera* cameraObject, physicat::core::LifeObject* lifeObject) = 0;
    };
}


#endif //PHYSICAT_RENDERER_HPP
