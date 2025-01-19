//
// Created by Akira Mujawar on 22/06/24.
//

#ifndef PHYSICAT_OPENGL_LINE_PIPELINE_HPP
#define PHYSICAT_OPENGL_LINE_PIPELINE_HPP

#include "glm_wrapper.hpp"
#include "graphics_wrapper.hpp"

#include "opengl_pipeline_base.hpp"
#include "line_render_component.hpp"
#include "transform3d_component.hpp"
#include "perspective_camera.hpp"

using namespace std;
using namespace glm;

namespace MeowEngine::pipeline {
    struct OpenGLLinePipeline : public MeowEngine::pipeline::OpenGLPipelineBase {
        OpenGLLinePipeline(const GLuint& shaderProgramID);
        ~OpenGLLinePipeline() override;

    public:
        void Render(
            const MeowEngine::OpenGLAssetManager& assetManager,
            const MeowEngine::entity::LineRenderComponent* lineRenderComponent,
            const MeowEngine::entity::Transform3DComponent* transform3DComponent,
            const MeowEngine::PerspectiveCamera* camera
        ) const;

    private:
        const GLuint ShaderProgramID;
        unsigned int VertexBufferID, VertexArrayID;
    };
}

#endif //PHYSICAT_OPENGL_LINE_PIPELINE_HPP
