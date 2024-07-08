//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_OPENGL_ASSET_MANAGER_HPP
#define PHYSICAT_OPENGL_ASSET_MANAGER_HPP

#include "asset_manager.hpp"
#include "src/utils/internal_ptr.hpp"
//#include "opengl_pipeline_base.hpp"
//#include "opengl_mesh_pipeline.hpp"
#include "opengl_mesh.hpp"
#include "opengl_texture.hpp"

namespace physicat {
    struct OpenGLAssetManager : public AssetManager {
        OpenGLAssetManager();

        // This forces for assets to load before application starts
        // This way we can apply optimization techniques like bundling or using single memory buffer
        // TODO: checkout bin-packing for texture optimization
        void LoadShaderPipelines(const std::vector<physicat::assets::ShaderPipelineType>& shaderPipelines) override;
        void LoadStaticMeshes(const std::vector<physicat::assets::StaticMeshType>& staticMeshes) override;
        void LoadTextures(const std::vector<physicat::assets::TextureType>& textures) override;

        template<typename T>
        T* GetShaderPipeline(const physicat::assets::ShaderPipelineType& shaderPipeline);
        const physicat::OpenGLMesh& GetStaticMesh(const physicat::assets::StaticMeshType& staticMesh) const;
        const physicat::OpenGLTexture& GetTexture(const physicat::assets::TextureType& texture) const;

    private:
        // We are using this because we need to store the state in order to cache the assets
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_OPENGL_ASSET_MANAGER_HPP
