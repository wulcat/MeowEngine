//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef MEOWENGINE_SKY_BOX_COMPONENT_BASE_HPP
#define MEOWENGINE_SKY_BOX_COMPONENT_BASE_HPP

//#include "render_component_base.hpp"

#include "render_component_base.hpp"

namespace MeowEngine::entity {
    // This class is extended for mesh, debug & ui rendering
    class SkyBoxComponent : public MeowEngine::entity::RenderComponentBase {

    public:
        static void Reflect();

        explicit SkyBoxComponent();
        explicit SkyBoxComponent(MeowEngine::assets::ShaderPipelineType shaderPipelineType);
        virtual ~SkyBoxComponent() = default;

//        virtual void Update(const glm::mat4 &projectionViewMatrix) = 0;

        MeowEngine::assets::ShaderPipelineType& GetShaderPipelineType() { return Shader; }

    protected:
        MeowEngine::assets::ShaderPipelineType Shader;
    };
}

#endif //MEOWENGINE_SKY_BOX_COMPONENT_BASE_HPP
