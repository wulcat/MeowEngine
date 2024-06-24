//
// Created by Akira Mujawar on 22/06/24.
//

#include "opengl_line_pipeline.hpp"

using physicat::pipeline::OpenGLLinePipeline;

OpenGLLinePipeline::OpenGLLinePipeline(const GLuint &shaderProgramID)
    : ShaderProgramID(shaderProgramID) {
    glGenVertexArrays(1, &VertexArrayID);
    glGenBuffers(1, &VertexBufferID);
}

OpenGLLinePipeline::~OpenGLLinePipeline() {
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &VertexBufferID);
    glDeleteProgram(ShaderProgramID);
}

void OpenGLLinePipeline::Render(
    const physicat::OpenGLAssetManager &assetManager,
    const physicat::core::component::LineRenderComponent *lineRenderComponent) const {

    glUseProgram(ShaderProgramID);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineRenderComponent->Vertices), lineRenderComponent->Vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "MVP"), 1, GL_FALSE, &lineRenderComponent->ProjectionViewMatrix[0][0]);
    glUniform3fv(glGetUniformLocation(ShaderProgramID, "color"), 1, &lineRenderComponent->LineColor[0]);

    glBindVertexArray(VertexArrayID);
    glDrawArrays(GL_LINES, 0, 2);
}