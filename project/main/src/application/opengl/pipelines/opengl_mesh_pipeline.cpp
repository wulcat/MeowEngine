//
// Created by Akira Mujawar on 23/06/24.
//

#include "opengl_mesh_pipeline.hpp"

//#include "opengl_pipeline_base.hpp"
#include "src/core/assets/assets.hpp"
#include "opengl_asset_manager.hpp"
#include <stdexcept>
//#include <vector>

using physicat::pipeline::OpenGLMeshPipeline;


OpenGLMeshPipeline::OpenGLMeshPipeline(const GLuint& shaderProgramID)
    : ShaderProgramID(shaderProgramID)
    , UniformLocationMVP(glGetUniformLocation(ShaderProgramID, "u_mvp"))
    , AttributeLocationVertexPosition(glGetAttribLocation(ShaderProgramID, "a_vertexPosition"))
    , AttributeLocationTextureCoord(glGetAttribLocation(ShaderProgramID, "a_textureCoord"))
    , Stride(5 * sizeof (float))
    , OffsetPosition(0)
    , OffsetTextureCoord(3 * sizeof(float)) {

}

OpenGLMeshPipeline::~OpenGLMeshPipeline() {
    glDeleteProgram(ShaderProgramID);
}

void OpenGLMeshPipeline::Render(
        const physicat::OpenGLAssetManager &assetManager,
        const physicat::core::component::MeshRenderComponent *meshRenderComponent) const {

    // start - Activate Shader Program
    // Tell opengl to use our shader program
    glUseProgram(ShaderProgramID);

//#ifndef USING_GLES
//        // This will render a wire frame for us :)
//        // n just like that we can have a wire frame mode (not that i need it but yeah lol)
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//#endif
    // Activating our vertex position attribute
    glEnableVertexAttribArray(AttributeLocationVertexPosition);

    // Activate our texture coord attribute
    glEnableVertexAttribArray(AttributeLocationTextureCoord);
    // end - Activate Shader Program - finish

    const physicat::StaticMeshInstance& staticMeshInstance = meshRenderComponent->GetMeshInstance();

//    for(const auto& staticMeshInstance : staticMeshInstances) {
        const physicat::OpenGLMesh& mesh = assetManager.GetStaticMesh(staticMeshInstance.GetMesh());

        // Populating our MVP in shader program
        glUniformMatrix4fv(UniformLocationMVP, 1, GL_FALSE, &staticMeshInstance.GetTransformMatrix()[0][0]);

        // Apply the texture we want to paint the mesh with.
        assetManager.GetTexture(staticMeshInstance.GetTexture()).Bind();

        // Bind the vertex and index buffers
        glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVertexBufferId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndexBufferId());

        // Configuring the vertex position attribute
        glVertexAttribPointer(
                AttributeLocationVertexPosition,
                3,
                GL_FLOAT,
                GL_FALSE,
                Stride,
                reinterpret_cast<const GLvoid*>(OffsetPosition) // (GLvoid*)(OffsetPosition) -> had warnings
        );

        // Config the texture coord
        glVertexAttribPointer(
                AttributeLocationTextureCoord,
                2,
                GL_FLOAT,
                GL_FALSE,
                Stride,
                reinterpret_cast<const GLvoid*>(OffsetTextureCoord) // (GLvoid*)(OffsetTextureCoord) -> had warnings
        );

        // Draw command providing the total number of vertices from mesh
        glDrawElements(
                GL_TRIANGLES,
                mesh.GetNumIndices(),
                GL_UNSIGNED_INT,
                reinterpret_cast<const GLvoid*>(0) // (GLvoid*)(0)
        );

//    }

    // Disabling the vertex position attribute as we are done using it. (seems like file open - close streaming process)
    glDisableVertexAttribArray(AttributeLocationVertexPosition);
    glDisableVertexAttribArray(AttributeLocationTextureCoord);
}