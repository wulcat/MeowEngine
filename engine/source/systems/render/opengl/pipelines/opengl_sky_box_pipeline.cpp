//
// Created by Akira Mujawar on 22/06/24.
//

#include "opengl_sky_box_pipeline.hpp"


using MeowEngine::pipeline::OpenGLSkyBoxPipeline;

OpenGLSkyBoxPipeline::OpenGLSkyBoxPipeline(const GLuint &shaderProgramID)
        : ShaderProgramID(shaderProgramID) {
    glGenVertexArrays(1, &VertexArrayID);
//    StartTime = high_resolution_clock::now();
}

OpenGLSkyBoxPipeline::~OpenGLSkyBoxPipeline() {
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(ShaderProgramID);
}

void OpenGLSkyBoxPipeline::Render(
        const MeowEngine::OpenGLAssetManager &assetManager,
        const MeowEngine::entity::SkyBoxComponent *renderComponent,
        const MeowEngine::entity::Transform3DComponent* transform3DComponent,
        const MeowEngine::PerspectiveCamera* camera) const {

    glUseProgram(ShaderProgramID);

//    auto now = high_resolution_clock::now();
//    duration<float> elapsedTime = now - StartTime;

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_view"), 1, GL_FALSE, &camera->GetViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_projection"), 1, GL_FALSE, &camera->GetProjectionMatrix()[0][0]);
//    glUniform1f(glGetUniformLocation(ShaderProgramID, "u_time"), elapsedTime.count());

    glBindVertexArray(VertexArrayID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}