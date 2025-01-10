//
// Created by Akira Mujawar on 23/06/24.
//

#include "render_component_base.hpp"
#include <log.hpp>

using physicat::entity::RenderComponentBase;

void physicat::entity::RenderComponentBase::Reflect() {
//    REGISTER_PROPERTY(Transform3DComponent, Position, physicat::math::Vector3);

    physicat::Log("Reflected", "RenderComponentBase");
}

RenderComponentBase::RenderComponentBase()
    : Shader(physicat::assets::ShaderPipelineType::Default) {}

RenderComponentBase::RenderComponentBase(physicat::assets::ShaderPipelineType shaderPipelineType)
    : Shader(shaderPipelineType) {}
