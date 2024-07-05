//
// Created by Akira Mujawar on 22/06/24.
//

#include "opengl_grid_pipeline.hpp"

using physicat::pipeline::OpenGLGridPipeline;

OpenGLGridPipeline::OpenGLGridPipeline(const GLuint &shaderProgramID)
        : ShaderProgramID(shaderProgramID) {
    glGenVertexArrays(1, &VertexArrayID);
}

OpenGLGridPipeline::~OpenGLGridPipeline() {
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(ShaderProgramID);
}

void OpenGLGridPipeline::Render(
        const physicat::OpenGLAssetManager &assetManager,
        const physicat::core::component::RenderComponentBase *renderComponent,
        const physicat::core::component::Transform3DComponent* transform3DComponent,
        const physicat::PerspectiveCamera* camera) const {

    glUseProgram(ShaderProgramID);
//    glBindVertexArray(VertexArrayID);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);

//    physicat::Log("test", std::to_string(VertexArrayID));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_view"), 1, GL_FALSE, &camera->GetViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_projection"), 1, GL_FALSE, &camera->GetProjectionMatrix()[0][0]);
//    glUniform3fv(glGetUniformLocation(ShaderProgramID, "position"), 1, &transform3DComponent->Position[0]);

//    glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
//    glEnableVertexAttribArray(0);

    glBindVertexArray(VertexArrayID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDrawElements(
//            GL_TRIANGLES,
//            3,
//            GL_UNSIGNED_INT,
//            reinterpret_cast<const GLvoid*>(0) // (GLvoid*)(0)
//    );

}