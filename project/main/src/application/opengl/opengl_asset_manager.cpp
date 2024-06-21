//
// Created by Akira Mujawar on 21/06/24.
//

#include "opengl_asset_manager.hpp"

using physicat::OpenGLAssetManager;

struct OpenGLAssetManager::Internal {
    // cache and store using enums
    std::unordered_map<physicat::assets::ShaderPipelineType, physicat::OpenGLPipeline> shaderPipelineCache;
    std::unordered_map<physicat::assets::StaticMeshType, physicat::OpenGLMesh> staticMeshCache;
    std::unordered_map<physicat::assets::TextureType, physicat::OpenGLTexture> textureCache;

    Internal() {}

    void LoadShaderPipelines(const std::vector<physicat::assets::ShaderPipelineType>& shaderPipelines) {
        for(const auto& shaderPipeline : shaderPipelines) {
            if(shaderPipelineCache.count(shaderPipeline) == 0) {
                shaderPipelineCache.insert(
                    std::make_pair(
                        shaderPipeline,
                        physicat::OpenGLPipeline(physicat::assets::ResolveShaderPipelinePath(shaderPipeline))
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

const physicat::OpenGLPipeline& OpenGLAssetManager::GetShaderPipeline(const physicat::assets::ShaderPipelineType& shaderPipeline) const {
    return InternalPointer->shaderPipelineCache.at(shaderPipeline);
}

const physicat::OpenGLMesh& OpenGLAssetManager::GetStaticMesh(const physicat::assets::StaticMeshType& staticMesh) const {
    return InternalPointer->staticMeshCache.at(staticMesh);
}

const physicat::OpenGLTexture& OpenGLAssetManager::GetTexture(const physicat::assets::TextureType& texture) const {
    return InternalPointer->textureCache.at(texture);
}




