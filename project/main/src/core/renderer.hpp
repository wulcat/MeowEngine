//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_RENDERER_HPP
#define PHYSICAT_RENDERER_HPP

#pragma once

#include "asset_inventory.hpp"
#include "static_mesh_instance.hpp"
#include <vector>

namespace physicat {
    struct Renderer {
        virtual void Render(
            const physicat::assets::ShaderPipelineType& shaderPipelineType,
            const std::vector<physicat::StaticMeshInstance>& staticMeshInstances
        ) = 0;
    };
}


#endif //PHYSICAT_RENDERER_HPP
