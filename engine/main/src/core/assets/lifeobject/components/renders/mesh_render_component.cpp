//
// Created by Akira Mujawar on 23/06/24.
//

#include "mesh_render_component.hpp"

using physicat::core::component::MeshRenderComponent;

MeshRenderComponent::MeshRenderComponent(physicat::assets::ShaderPipelineType shader, physicat::StaticMeshInstance *meshInstance)
    : MeshInstance(meshInstance) {
    Shader = shader;
}