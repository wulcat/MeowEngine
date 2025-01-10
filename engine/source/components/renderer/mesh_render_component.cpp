//
// Created by Akira Mujawar on 23/06/24.
//

#include "mesh_render_component.hpp"
#include <log.hpp>
#include "entt_reflection_wrapper.hpp"

using physicat::entity::MeshRenderComponent;

void physicat::entity::DummyClass::Reflect() {
    REGISTER_PROPERTY(DummyClass, MeshType, int);
    REGISTER_PROPERTY(DummyClass, Size, float);
}

void physicat::entity::MeshRenderComponent::Reflect() {
    REGISTER_PROPERTY(MeshRenderComponent, Data, DummyClass);

    physicat::Log("Reflected", "MeshRenderComponent");
}

MeshRenderComponent::MeshRenderComponent(physicat::assets::ShaderPipelineType shader, physicat::StaticMeshInstance *meshInstance)
    : MeshInstance(meshInstance), Data() {
    Shader = shader;
}