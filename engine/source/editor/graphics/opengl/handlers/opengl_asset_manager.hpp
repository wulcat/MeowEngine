//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_OPENGL_ASSET_MANAGER_HPP
#define PHYSICAT_OPENGL_ASSET_MANAGER_HPP

#include "asset_manager.hpp"
#include "internal_ptr.hpp"
//#include "opengl_pipeline_base.hpp"
//#include "opengl_mesh_pipeline.hpp"
#include "opengl_mesh.hpp"
#include "opengl_texture.hpp"

namespace MeowEngine {
    struct OpenGLAssetManager : public AssetManager {
        OpenGLAssetManager();

        // This forces for assets to load before application starts
        // This way we can apply optimization techniques like bundling or using single memory buffer
        // TODO: checkout bin-packing for texture optimization
        void LoadShaderPipelines(const std::vector<MeowEngine::assets::ShaderPipelineType>& shaderPipelines) override;
        void LoadStaticMeshes(const std::vector<MeowEngine::assets::StaticMeshType>& staticMeshes) override;
        void LoadTextures(const std::vector<MeowEngine::assets::TextureType>& textures) override;

        template<typename T>
        T* GetShaderPipeline(const MeowEngine::assets::ShaderPipelineType& shaderPipeline);
        const MeowEngine::OpenGLMesh& GetStaticMesh(const MeowEngine::assets::StaticMeshType& staticMesh) const;
        const MeowEngine::OpenGLTexture& GetTexture(const MeowEngine::assets::TextureType& texture) const;

    private:
        // We are using this because we need to store the state in order to cache the assets
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_OPENGL_ASSET_MANAGER_HPP
