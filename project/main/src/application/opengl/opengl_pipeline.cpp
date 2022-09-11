//
// Created by Akira Lynn on 11/09/22.
//

#include "opengl_pipeline.hpp"
#include "../../core/assets/assets.hpp"
#include "../../core/logger/log.hpp"
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

    explicit Internal(const std::string& shaderName)
        : ShaderProgramId(CreateShaderProgram(shaderName))
        , UniformLocationMVP(glGetUniformLocation(ShaderProgramId, "mvp"))
        , AttributeLocationVertexPosition(glGetAttribLocation(ShaderProgramId, "vertexPosition"))
        {}

    ~Internal() {
        glDeleteProgram(ShaderProgramId);
    }
};

OpenGLPipeline::OpenGLPipeline(const std::string& shaderName)
    : InternalPointer(physicat::make_internal_ptr<Internal>(shaderName)) {}
