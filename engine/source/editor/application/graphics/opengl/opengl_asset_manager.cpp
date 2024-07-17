//
// Created by Akira Mujawar on 21/06/24.
//

#include "opengl_asset_manager.hpp"

#include "opengl_pipeline_base.hpp"
#include "opengl_mesh_pipeline.hpp"
#include "opengl_line_pipeline.hpp"
#include "opengl_grid_pipeline.hpp"


using physicat::OpenGLAssetManager;

using namespace physicat::assets;
using namespace physicat::pipeline;

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
        const std::string logTag{"physicat::OpenGLAssetManager::CreateShaderProgram"};
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
//        std::string vertexShaderSource {"#version 140\n" + vertexShaderCode};
//        std::string fragmentShaderSource{"#version 140\n" + fragmentShaderCode};
        std::string vertexShaderSource {"#version 330 core\n" + vertexShaderCode};
        std::string fragmentShaderSource{"#version 330 core\n" + fragmentShaderCode};
//        std::string vertexShaderSource {vertexShaderCode};
//        std::string fragmentShaderSource{fragmentShaderCode};
#endif

        // Compile fragment and vertex shader from our compiler (method)
        GLuint shaderProgramId {glCreateProgram()};
        GLuint vertexShaderId {CompileShader(GL_VERTEX_SHADER, vertexShaderSource)};
        GLuint fragmentShaderId {CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource)};

        glAttachShader(shaderProgramId, vertexShaderId);
        glAttachShader(shaderProgramId, fragmentShaderId);
        glLinkProgram(shaderProgramId);

        // Link Shader
        GLint shaderProgramLinkResult;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &shaderProgramLinkResult);

        // Check the if our shader was successfully linked
        if(!shaderProgramLinkResult) {
            GLint errorMessageLength;
            glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &errorMessageLength);
            std::vector<char> errorMessage(errorMessageLength + 1);
            glGetProgramInfoLog(shaderProgramId, errorMessageLength, nullptr, &errorMessage[0]);
            physicat::Log(logTag, &errorMessage[0]);

            throw std::runtime_error(logTag + "Shader Program failed to compile");
        }

        // It's important to detach shader from that way deleting shader free's the memory
        glDetachShader(shaderProgramId, vertexShaderId);
        glDetachShader(shaderProgramId, fragmentShaderId);

        // Clean up shader as they are no longer needed after linking
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return shaderProgramId;
    }

    physicat::pipeline::OpenGLPipelineBase* CreatePipeline(const ShaderPipelineType& shaderPipelineType) {
        const std::string shaderPath = physicat::assets::ResolveShaderPipelinePath(shaderPipelineType);
        GLuint shaderProgramID = CreateShaderProgram(shaderPath);

        switch (shaderPipelineType) {
            case ShaderPipelineType::Default:
                return new OpenGLMeshPipeline(shaderProgramID);
            case ShaderPipelineType::Line:
                return new OpenGLLinePipeline(shaderProgramID);
            case ShaderPipelineType::Grid:
                return new OpenGLGridPipeline(shaderProgramID);
            default:
                return {};
        }
    }

} // namespace

struct OpenGLAssetManager::Internal {
    // cache and store using enums
    std::unordered_map<physicat::assets::ShaderPipelineType, physicat::pipeline::OpenGLPipelineBase*> shaderPipelineCache;
    std::unordered_map<physicat::assets::StaticMeshType, physicat::OpenGLMesh> staticMeshCache;
    std::unordered_map<physicat::assets::TextureType, physicat::OpenGLTexture> textureCache;

    Internal() {}

    void LoadShaderPipelines(const std::vector<physicat::assets::ShaderPipelineType>& shaderPipelines) {
        for(const auto& shaderPipeline : shaderPipelines) {
            if(shaderPipelineCache.count(shaderPipeline) == 0) {
                shaderPipelineCache.insert(
                    std::make_pair(
                        shaderPipeline,
                        ::CreatePipeline(shaderPipeline)
                    )
                );
            }
        }
    }

    void LoadStaticMeshes(const std::vector<physicat::assets::StaticMeshType>& staticMeshes) {
        for(const auto& staticMesh : staticMeshes) {
            if(staticMeshCache.count(staticMesh) == 0) {
                staticMeshCache.insert(
                    std::make_pair(
                        staticMesh,
                        physicat::OpenGLMesh(physicat::assets::LoadObjFile(physicat::assets::ResolveStaticMeshPath(staticMesh)))
                    )
                );
            }
        }
    }

    void LoadTextures(const std::vector<physicat::assets::TextureType>& textures) {
        for(const auto& texture : textures) {
            if(textureCache.count(texture) == 0) {
                textureCache.insert(
                    std::make_pair(
                        texture,
                        physicat::OpenGLTexture(physicat::assets::LoadBitmap(physicat::assets::ResolveTexturePath(texture)))
                    )
                );
            }
        }
    }
};

OpenGLAssetManager::OpenGLAssetManager()
    : InternalPointer(physicat::make_internal_ptr<Internal>()) {}

void OpenGLAssetManager::LoadShaderPipelines(const std::vector<physicat::assets::ShaderPipelineType> &shaderPipelines) {
    InternalPointer->LoadShaderPipelines(shaderPipelines);
}

void OpenGLAssetManager::LoadStaticMeshes(const std::vector<physicat::assets::StaticMeshType> &staticMeshes) {
    InternalPointer->LoadStaticMeshes(staticMeshes);
}

void OpenGLAssetManager::LoadTextures(const std::vector<physicat::assets::TextureType> &textures) {
    InternalPointer->LoadTextures(textures);
}

using physicat::pipeline::OpenGLMeshPipeline;

// using template for having dynamic return of pipelines
template<typename T>
T* OpenGLAssetManager::GetShaderPipeline(const physicat::assets::ShaderPipelineType& shaderPipeline) {
    auto test = InternalPointer->shaderPipelineCache.at(shaderPipeline);

    return static_cast<T*>(test);
}

template OpenGLMeshPipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLMeshPipeline>(const physicat::assets::ShaderPipelineType& shaderPipeline);
template OpenGLLinePipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLLinePipeline>(const physicat::assets::ShaderPipelineType& shaderPipeline);
template OpenGLGridPipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLGridPipeline>(const physicat::assets::ShaderPipelineType& shaderPipeline);


const physicat::OpenGLMesh& OpenGLAssetManager::GetStaticMesh(const physicat::assets::StaticMeshType& staticMesh) const {
    return InternalPointer->staticMeshCache.at(staticMesh);
}

const physicat::OpenGLTexture& OpenGLAssetManager::GetTexture(const physicat::assets::TextureType& texture) const {
    return InternalPointer->textureCache.at(texture);
}




