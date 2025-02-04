//
// Created by Akira Mujawar on 04/02/25.
//

#include "opengl_collider_pipeline.hpp"

namespace MeowEngine {
    OpenGLCollidePipeline::OpenGLCollidePipeline(const GLuint &inShaderProgramID)
    : ShaderProgramID(inShaderProgramID) {}
    OpenGLCollidePipeline::~OpenGLCollidePipeline() noexcept {}

    void OpenGLCollidePipeline::Render() {}
} // MeowEngine