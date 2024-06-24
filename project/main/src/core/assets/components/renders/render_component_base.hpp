//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_RENDER_COMPONENT_BASE_HPP
#define PHYSICAT_RENDER_COMPONENT_BASE_HPP

#include "component_base.hpp"
//#include <static_mesh_instance.hpp>
#include "asset_inventory.hpp"

namespace physicat::core::component {
    // This class is extended for mesh, debug & ui rendering
    class RenderComponentBase : public physicat::core::component::ComponentBase {

    public:
        RenderComponentBase();
        virtual ~RenderComponentBase() = default;

        virtual void Update(const glm::mat4 &projectionViewMatrix) = 0;

        physicat::assets::ShaderPipelineType& GetShaderPipelineType() { return Shader; }

    protected:
        physicat::assets::ShaderPipelineType Shader;
    };
}

#endif //PHYSICAT_RENDER_COMPONENT_BASE_HPP
