//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef MEOWENGINE_LINE_RENDER_COMPONENT_HPP
#define MEOWENGINE_LINE_RENDER_COMPONENT_HPP

#include "render_component_base.hpp"

// the grid will come in this
namespace MeowEngine::entity {
    class LineRenderComponent : public MeowEngine::entity::RenderComponentBase {
    public:
        static void Reflect();

        explicit LineRenderComponent(MeowEngine::assets::ShaderPipelineType shader, glm::vec3 start, glm::vec3 end);

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


#endif //MEOWENGINE_LINE_RENDER_COMPONENT_HPP
