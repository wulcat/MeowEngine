//
// Created by Akira Lynn on 11/09/22.
//

#include "opengl_pipeline.hpp"
#include "../../core/assets/assets.hpp"
#include "../../core/logger/log.hpp"
#include "opengl_asset_manager.hpp"
#include <stdexcept>
#include <vector>

using physicat::OpenGLPipeline;

namespace {
    GLuint CompileShader(const GLenum& shaderType, const std::string& shaderSource) {
        const std::string logTag {"physicat::OpenGLPipeline::CompileShader"};
        GLuint shaderId{glCreateShader(shaderType)};

        const char* shaderData{shaderSource.c_str()};
        glShaderSource(shaderId, 1, &shaderData, nullptr);
        glCompileShader(shaderId);

        GLint shaderCompilationResult;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompilationResult);

        if(!shaderCompilationResult) {
            GLint errorMessageLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
            std::vector<char> errorMessage(errorMessageLength + 1);
            glGetShaderInfoLog(shaderId, errorMessageLength, nullptr, &errorMessage[0]);
            physicat::Log(logTag, &errorMessage[0]);

            throw std::runtime_error(logTag + "Shader failed to compile");
        }

        return shaderId;
    }

    GLuint CreateShaderProgram(const std::string& shaderName) {
        const std::string logTag{"physicat::OpenGLPipeline::CreateShaderProgram"};
        physicat::Log(logTag, "Creating Pipeline for '" + shaderName + "'");

        const std::string vertexShaderCode {
            physicat::assets::LoadTextFile("assets/shaders/opengl/" + shaderName + ".vert")
        };
        const std::string fragmentShaderCode {
            physicat::assets::LoadTextFile("assets/shaders/opengl/" + shaderName + ".frag")
        };

#ifdef USING_GLES
        std::string vertexShaderSource {"#version 100\n" + vertexShaderCode};
        std::string fragmentShaderSource{"#version 100\nprecision mediump float;\n" + fragmentShaderCode};
#else
        std::string vertexShaderSource {"#version 120\n" + vertexShaderCode};
        std::string fragmentShaderSource{"#version 120\n" + fragmentShaderCode};
#endif

        GLuint shaderProgramId {glCreateProgram()};
        GLuint vertexShaderId {CompileShader(GL_VERTEX_SHADER, vertexShaderSource)};
        GLuint fragmentShaderId {CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource)};

        glAttachShader(shaderProgramId, vertexShaderId);
        glAttachShader(shaderProgramId, fragmentShaderId);
        glLinkProgram(shaderProgramId);

        GLint shaderProgramLinkResult;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &shaderProgramLinkResult);

        if(!shaderProgramLinkResult) {
            GLint errorMessageLength;
            glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &errorMessageLength);
            std::vector<char> errorMessage(errorMessageLength + 1);
            glGetProgramInfoLog(shaderProgramId, errorMessageLength, nullptr, &errorMessage[0]);
            physicat::Log(logTag, &errorMessage[0]);

            throw std::runtime_error(logTag + "Shader Program failed to compile");
        }

        glDetachShader(shaderProgramId, vertexShaderId);
        glDetachShader(shaderProgramId, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return shaderProgramId;
    }

} // namespace

struct OpenGLPipeline::Internal {
    const GLuint ShaderProgramId;
    const GLuint UniformLocationMVP;
    const GLuint AttributeLocationVertexPosition;
    const GLuint AttributeLocationTextureCoord;
    const GLsizei Stride;
    const GLsizei OffsetPosition;
    const GLsizei OffsetTextureCoord;

    explicit Internal(const std::string& shaderName)
        : ShaderProgramId(CreateShaderProgram(shaderName))
        , UniformLocationMVP(glGetUniformLocation(ShaderProgramId, "u_mvp"))
        , AttributeLocationVertexPosition(glGetAttribLocation(ShaderProgramId, "a_vertexPosition"))
        , AttributeLocationTextureCoord(glGetAttribLocation(ShaderProgramId, "a_textureCoord"))
        , Stride(5 * sizeof (float))
        , OffsetPosition(0)
        , OffsetTextureCoord(3 * sizeof(float))
        {}

    void Render(
            const physicat::OpenGLAssetManager& assetManager,
            const std::vector<physicat::StaticMeshInstance>& staticMeshInstances
    ) const {
//#ifndef USING_GLES
//        // This will render a wire frame for us :)
//        // n just like that we can have a wire frame mode (not that i need it but yeah lol)
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//#endif

        // Activate Shader Program

        // Tell opengl to use our shader program
        glUseProgram(ShaderProgramId);

        // Activating our vertex position attribute
        glEnableVertexAttribArray(AttributeLocationVertexPosition);

        // Activate our texture coord attribute
        glEnableVertexAttribArray(AttributeLocationTextureCoord);

        // Activate Shader Program - finish

        for(const auto& staticMeshInstance : staticMeshInstances) {
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

        }

        // Disabling the vertex position attribute as we are done using it. (seems like file open - close streaming process)
        glDisableVertexAttribArray(AttributeLocationVertexPosition);
        glDisableVertexAttribArray(AttributeLocationTextureCoord);
    }

    ~Internal() {
        glDeleteProgram(ShaderProgramId);
    }
};

OpenGLPipeline::OpenGLPipeline(const std::string& shaderName)
    : InternalPointer(physicat::make_internal_ptr<Internal>(shaderName)) {}

void physicat::OpenGLPipeline::Render(
        const physicat::OpenGLAssetManager& assetManager,
        const std::vector<physicat::StaticMeshInstance>& staticMeshInstances
) const {
    InternalPointer->Render(assetManager, staticMeshInstances);
}
