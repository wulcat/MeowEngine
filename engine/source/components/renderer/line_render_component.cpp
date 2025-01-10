//
// Created by Akira Mujawar on 23/06/24.
//

#include "line_render_component.hpp"
#include <log.hpp>

using physicat::entity::LineRenderComponent;

void physicat::entity::LineRenderComponent::Reflect() {
//    REGISTER_PROPERTY(Transform3DComponent, Position, physicat::math::Vector3);

    physicat::Log("Reflected", "LineRenderComponent");
}

LineRenderComponent::LineRenderComponent(physicat::assets::ShaderPipelineType shader, glm::vec3 start, glm::vec3 end) {
    Shader = shader;
    StartPoint = start;
    EndPoint = end;
    LineColor = glm::vec4(0.4f,1,1,0.3f);

    Vertices = {
        start.x, start.y, start.z,
        end.x, end.y, end.z,
    };
}