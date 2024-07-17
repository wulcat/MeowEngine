//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_LINE_RENDER_COMPONENT_HPP
#define PHYSICAT_LINE_RENDER_COMPONENT_HPP

#include "render_component_base.hpp"

// the grid will come in this
namespace physicat::core::component {
    class LineRenderComponent : public physicat::core::component::RenderComponentBase {
    public:
        explicit LineRenderComponent(physicat::assets::ShaderPipelineType shader, glm::vec3 start, glm::vec3 end);

//        void Update(const glm::mat4 &projectionViewMatrix) override {
////            MeshInstance->Update(projectionViewMatrix);
////            MeshInstance->RotateBy(0.1f);
//            ProjectionViewMatrix = projectionViewMatrix;
//        }

    public:
        glm::vec3 StartPoint;
        glm::vec3 EndPoint;
        glm::vec4 LineColor;
        std::vector<float> Vertices;
//        glm::mat4 ProjectionViewMatrix;
    };
}


#endif //PHYSICAT_LINE_RENDER_COMPONENT_HPP
