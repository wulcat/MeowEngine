//
// Created by Akira Mujawar on 23/06/24.
//

#include "mesh_render_component.hpp"
#include <log.hpp>
#include "entt_reflection_wrapper.hpp"

using MeowEngine::entity::MeshRenderComponent;

void MeowEngine::entity::DummyClass::Reflect() {
    REGISTER_PROPERTY(DummyClass, MeshType, int);
    REGISTER_PROPERTY(DummyClass, Size, float);
}

void MeowEngine::entity::MeshRenderComponent::Reflect() {
    REGISTER_PROPERTY(MeshRenderComponent, Data, DummyClass);

    MeowEngine::Log("Reflected", "MeshRenderComponent");
}

MeshRenderComponent::MeshRenderComponent(MeowEngine::assets::ShaderPipelineType shader, MeowEngine::StaticMeshInstance *meshInstance)
    : MeshInstance(meshInstance), Data() {
    Shader = shader;
}