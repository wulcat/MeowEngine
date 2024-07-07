//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_ASSET_MANAGER_HPP
#define PHYSICAT_ASSET_MANAGER_HPP

#include "asset_inventory.hpp"
#include "vector"

namespace physicat {
    struct AssetManager {
        virtual void LoadShaderPipelines(const std::vector<physicat::assets::ShaderPipelineType>& shaderPipelines) = 0;
        virtual void LoadStaticMeshes(const std::vector<physicat::assets::StaticMeshType>& staticMeshes) = 0;
        virtual void LoadTextures(const std::vector<physicat::assets::TextureType>& textures) = 0;
    };
}


#endif //PHYSICAT_ASSET_MANAGER_HPP
