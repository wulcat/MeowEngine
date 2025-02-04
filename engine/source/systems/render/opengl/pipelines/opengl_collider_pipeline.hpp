//
// Created by Akira Mujawar on 04/02/25.
//

#ifndef MEOWENGINE_OPENGL_COLLIDER_PIPELINE_HPP
#define MEOWENGINE_OPENGL_COLLIDER_PIPELINE_HPP

#include "glm_wrapper.hpp"
#include "graphics_wrapper.hpp"

#include "opengl_pipeline_base.hpp"

namespace MeowEngine {

    struct OpenGLCollidePipeline : public MeowEngine::pipeline::OpenGLPipelineBase {
        OpenGLCollidePipeline(const GLuint& inShaderProgramID);
        ~OpenGLCollidePipeline() override;

    public:
        void Render();

    private:
        const GLuint ShaderProgramID;
    };

} // MeowEngine

#endif //MEOWENGINE_OPENGL_COLLIDER_PIPELINE_HPP
