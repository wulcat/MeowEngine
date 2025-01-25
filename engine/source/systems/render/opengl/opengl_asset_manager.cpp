//
// Created by Akira Mujawar on 21/06/24.
//

#include "opengl_asset_manager.hpp"

#include "opengl_pipeline_base.hpp"
#include "opengl_mesh_pipeline.hpp"
#include "opengl_line_pipeline.hpp"
#include "opengl_grid_pipeline.hpp"
#include "opengl_sky_box_pipeline.hpp"


using MeowEngine::OpenGLAssetManager;

using namespace MeowEngine::assets;
using namespace MeowEngine::pipeline;

namespace {
    GLuint CompileShader(const GLenum& shaderType, const std::string& shaderSource) {
        const std::string logTag {"MeowEngine::OpenGLPipeline::CompileShader"};
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
            MeowEngine::Log(logTag, &errorMessage[0]);

            throw std::runtime_error(logTag + "Shader failed to compile");
        }

        return shaderId;
    }

    GLuint CreateShaderProgram(const std::string& shaderName) {
        const std::string logTag{"MeowEngine::OpenGLAssetManager::CreateShaderProgram"};
        MeowEngine::Log(logTag, "Creating Pipeline for '" + shaderName + "'");

        const std::string vertexShaderCode {
                MeowEngine::assets::LoadTextFile("assets/shaders/opengl/" + shaderName + ".vert")
        };
        const std::string fragmentShaderCode {
                MeowEngine::assets::LoadTextFile("assets/shaders/opengl/" + shaderName + ".frag")
        };

#ifdef USING_GLES
        MeowEngine::Log(logTag, "#version 300 es") ;
//        std::string vertexShaderSource {"#version 100\n" + vertexShaderCode};
//        std::string fragmentShaderSource{"#version 100\nprecision mediump float;\n" + fragmentShaderCode};
        std::string vertexShaderSource {"#version 300 es\nprecision mediump float;\n" + vertexShaderCode};
        std::string fragmentShaderSource{"#version 300 es\nprecision mediump float;\n" + fragmentShaderCode};
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
            MeowEngine::Log(logTag, &errorMessage[0]);

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

    MeowEngine::pipeline::OpenGLPipelineBase* CreatePipeline(const ShaderPipelineType& shaderPipelineType) {
        const std::string shaderPath = MeowEngine::assets::ResolveShaderPipelinePath(shaderPipelineType);
        GLuint shaderProgramID = CreateShaderProgram(shaderPath);

        switch (shaderPipelineType) {
            case ShaderPipelineType::Default:
                return new OpenGLMeshPipeline(shaderProgramID);
            case ShaderPipelineType::Line:
                return new OpenGLLinePipeline(shaderProgramID);
            case ShaderPipelineType::Grid:
                return new OpenGLGridPipeline(shaderProgramID);
            case ShaderPipelineType::Sky:
                return new OpenGLGridPipeline(shaderProgramID);
            default:
                return {};
        }
    }

} // namespace

struct OpenGLAssetManager::Internal {
    // cache and store using enums
    std::unordered_map<MeowEngine::assets::ShaderPipelineType, MeowEngine::pipeline::OpenGLPipelineBase*> shaderPipelineCache;
    std::unordered_map<MeowEngine::assets::StaticMeshType, MeowEngine::OpenGLMesh> staticMeshCache;
    std::unordered_map<MeowEngine::assets::TextureType, MeowEngine::OpenGLTexture> textureCache;

    Internal() {}

    void LoadShaderPipelines(const std::vector<MeowEngine::assets::ShaderPipelineType>& shaderPipelines) {
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

    void LoadStaticMeshes(const std::vector<MeowEngine::assets::StaticMeshType>& staticMeshes) {
        for(const auto& staticMesh : staticMeshes) {
            if(staticMeshCache.count(staticMesh) == 0) {
                staticMeshCache.insert(
                    std::make_pair(
                        staticMesh,
                        MeowEngine::OpenGLMesh(MeowEngine::assets::LoadObjFile(MeowEngine::assets::ResolveStaticMeshPath(staticMesh)))
                    )
                );
            }
        }
    }

    void LoadTextures(const std::vector<MeowEngine::assets::TextureType>& textures) {
        for(const auto& texture : textures) {
            if(textureCache.count(texture) == 0) {
                textureCache.insert(
                    std::make_pair(
                        texture,
                        MeowEngine::OpenGLTexture(MeowEngine::assets::LoadBitmap(MeowEngine::assets::ResolveTexturePath(texture)))
                    )
                );
            }
        }
    }
};

OpenGLAssetManager::OpenGLAssetManager()
    : InternalPointer(MeowEngine::make_internal_ptr<Internal>()) {}

void OpenGLAssetManager::LoadShaderPipelines(const std::vector<MeowEngine::assets::ShaderPipelineType> &shaderPipelines) {
    InternalPointer->LoadShaderPipelines(shaderPipelines);
}

void OpenGLAssetManager::LoadStaticMeshes(const std::vector<MeowEngine::assets::StaticMeshType> &staticMeshes) {
    InternalPointer->LoadStaticMeshes(staticMeshes);
}

void OpenGLAssetManager::LoadTextures(const std::vector<MeowEngine::assets::TextureType> &textures) {
    InternalPointer->LoadTextures(textures);
}

using MeowEngine::pipeline::OpenGLMeshPipeline;

// using template for having dynamic return of pipelines
template<typename T>
T* OpenGLAssetManager::GetShaderPipeline(const MeowEngine::assets::ShaderPipelineType& shaderPipeline) {
    auto test = InternalPointer->shaderPipelineCache.at(shaderPipeline);

    return static_cast<T*>(test);
}

template OpenGLMeshPipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLMeshPipeline>(const MeowEngine::assets::ShaderPipelineType& shaderPipeline);
template OpenGLLinePipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLLinePipeline>(const MeowEngine::assets::ShaderPipelineType& shaderPipeline);
template OpenGLGridPipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLGridPipeline>(const MeowEngine::assets::ShaderPipelineType& shaderPipeline);
template OpenGLSkyBoxPipeline* OpenGLAssetManager::GetShaderPipeline<OpenGLSkyBoxPipeline>(const MeowEngine::assets::ShaderPipelineType& shaderPipeline);


const MeowEngine::OpenGLMesh& OpenGLAssetManager::GetStaticMesh(const MeowEngine::assets::StaticMeshType& staticMesh) const {
    return InternalPointer->staticMeshCache.at(staticMesh);
}

const MeowEngine::OpenGLTexture& OpenGLAssetManager::GetTexture(const MeowEngine::assets::TextureType& texture) const {
    return InternalPointer->textureCache.at(texture);
}




