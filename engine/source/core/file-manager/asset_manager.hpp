//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_ASSET_MANAGER_HPP
#define PHYSICAT_ASSET_MANAGER_HPP

#include "asset_inventory.hpp"
#include "vector"

namespace MeowEngine {
    struct AssetManager {
        virtual void LoadShaderPipelines(const std::vector<MeowEngine::assets::ShaderPipelineType>& shaderPipelines) = 0;
        virtual void LoadStaticMeshes(const std::vector<MeowEngine::assets::StaticMeshType>& staticMeshes) = 0;
        virtual void LoadTextures(const std::vector<MeowEngine::assets::TextureType>& textures) = 0;
    };
}


#endif //PHYSICAT_ASSET_MANAGER_HPP
