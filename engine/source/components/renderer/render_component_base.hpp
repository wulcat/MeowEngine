//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_RENDER_COMPONENT_BASE_HPP
#define PHYSICAT_RENDER_COMPONENT_BASE_HPP

#include "component_base.hpp"
//#include <static_mesh_instance.hpp>
#include "asset_inventory.hpp"

namespace MeowEngine::entity {
    // This class is extended for mesh, debug & ui rendering
    class RenderComponentBase : public MeowEngine::entity::ComponentBase {

    public:
        static void Reflect();

        RenderComponentBase();
        RenderComponentBase(MeowEngine::assets::ShaderPipelineType shaderPipelineType);
        virtual ~RenderComponentBase() = default;

//        virtual void Update(const glm::mat4 &projectionViewMatrix) = 0;

        MeowEngine::assets::ShaderPipelineType& GetShaderPipelineType() { return Shader; }

    protected:
        MeowEngine::assets::ShaderPipelineType Shader;
    };
}

#endif //PHYSICAT_RENDER_COMPONENT_BASE_HPP
