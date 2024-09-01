//
// Created by Akira Mujawar on 22/06/24.
//

#include "opengl_line_pipeline.hpp"

using physicat::pipeline::OpenGLLinePipeline;

OpenGLLinePipeline::OpenGLLinePipeline(const GLuint &shaderProgramID)
    : ShaderProgramID(shaderProgramID) {
    glGenVertexArrays(1, &VertexArrayID);
    glGenBuffers(1, &VertexBufferID);

//    // TODO: This needs to be executed only once (maybe our opengl render pipeline can set this on start)
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

OpenGLLinePipeline::~OpenGLLinePipeline() {
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &VertexBufferID);
    glDeleteProgram(ShaderProgramID);
}

void OpenGLLinePipeline::Render(
    const physicat::OpenGLAssetManager &assetManager,
    const physicat::entity::LineRenderComponent *lineRenderComponent,
    const physicat::entity::Transform3DComponent* transform3DComponent,
    const physicat::PerspectiveCamera* camera) const {

    glUseProgram(ShaderProgramID);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineRenderComponent->Vertices), lineRenderComponent->Vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // vertex shader
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_mvp"), 1, GL_FALSE, &transform3DComponent->TransformMatrix[0][0]);
    glUniform3fv(glGetUniformLocation(ShaderProgramID, "u_worldPosition"),1, &transform3DComponent->Position[0]);
    glUniform3fv(glGetUniformLocation(ShaderProgramID, "u_cameraPosition"), 1, &camera->GetPosition()[0]);

    // fragment shader
    glUniform3fv(glGetUniformLocation(ShaderProgramID, "u_color"), 1, &lineRenderComponent->LineColor[0]);
    glUniform1f(glGetUniformLocation(ShaderProgramID, "u_maxDistance"), 20.0f);

    glBindVertexArray(VertexArrayID);

    glDrawArrays(GL_LINES, 0, 2);
}