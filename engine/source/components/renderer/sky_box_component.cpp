//
// Created by Akira Mujawar on 23/06/24.
//

#include "sky_box_component.hpp"
#include <log.hpp>

using MeowEngine::entity::SkyBoxComponent;

void MeowEngine::entity::SkyBoxComponent::Reflect() {
    MeowEngine::Log("Reflected", "SkyBoxComponent");
}

SkyBoxComponent::SkyBoxComponent()
    : Shader(MeowEngine::assets::ShaderPipelineType::Default) {}

SkyBoxComponent::SkyBoxComponent(MeowEngine::assets::ShaderPipelineType shaderPipelineType)
    : Shader(shaderPipelineType) {}
