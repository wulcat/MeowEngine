//
// Created by Akira Mujawar on 22/06/24.
//

#ifndef MEOWENGINE_OPENGL_SKY_BOX_PIPELINE_HPP
#define MEOWENGINE_OPENGL_SKY_BOX_PIPELINE_HPP

#include "glm_wrapper.hpp"
#include "graphics_wrapper.hpp"

#include "opengl_pipeline_base.hpp"
#include "sky_box_component.hpp"
#include "transform3d_component.hpp"
#include "perspective_camera.hpp"
#include <chrono>
using namespace std::chrono;

using namespace std;
using namespace glm;

namespace MeowEngine::pipeline {
    struct OpenGLSkyBoxPipeline : public MeowEngine::pipeline::OpenGLPipelineBase {
        OpenGLSkyBoxPipeline(const GLuint& shaderProgramID);
        ~OpenGLSkyBoxPipeline() override;

    public:
        void Render(
            const MeowEngine::OpenGLAssetManager& assetManager,
            const MeowEngine::entity::SkyBoxComponent* renderComponent,
            const MeowEngine::entity::Transform3DComponent* transform3DComponent,
            const MeowEngine::PerspectiveCamera* camera
        ) const;

    private:
        const GLuint ShaderProgramID;
        unsigned int VertexArrayID;
//        time_point<high_resolution_clock> StartTime;
    };
}

#endif //MEOWENGINE_OPENGL_SKY_BOX_PIPELINE_HPP
