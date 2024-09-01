//
// Created by Akira Mujawar on 22/06/24.
//

#ifndef PHYSICAT_OPENGL_GRID_PIPELINE_HPP
#define PHYSICAT_OPENGL_GRID_PIPELINE_HPP

#include "glm_wrapper.hpp"
#include "graphics_wrapper.hpp"

#include "opengl_pipeline_base.hpp"
#include "render_component_base.hpp"
#include "transform3d_component.hpp"
#include "perspective_camera.hpp"

using namespace std;
using namespace glm;

namespace physicat::pipeline {
    struct OpenGLGridPipeline : public physicat::pipeline::OpenGLPipelineBase {
        OpenGLGridPipeline(const GLuint& shaderProgramID);
        ~OpenGLGridPipeline() override;

    public:
        void Render(
            const physicat::OpenGLAssetManager& assetManager,
            const physicat::entity::RenderComponentBase* renderComponent,
            const physicat::entity::Transform3DComponent* transform3DComponent,
            const physicat::PerspectiveCamera* camera
        ) const;

    private:
        const GLuint ShaderProgramID;
        unsigned int VertexArrayID;
    };
}

#endif //PHYSICAT_OPENGL_GRID_PIPELINE_HPP
