//
// Created by Akira Mujawar on 23/06/24.
//

#include "line_render_component.hpp"

using physicat::core::component::LineRenderComponent;

LineRenderComponent::LineRenderComponent(physicat::assets::ShaderPipelineType shader, glm::vec3 start, glm::vec3 end) {
    Shader = shader;
    StartPoint = start;
    EndPoint = end;

    LineColor = glm::vec3(1,0,0);
    ProjectionViewMatrix = glm::mat4(1.0f);

    Vertices = {
            start.x, start.y, start.z,
            end.x, end.y, end.z,
    };
}