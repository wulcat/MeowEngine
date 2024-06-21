//
// Created by Akira Mujawar on 21/06/24.
//

#include "asset_inventory.hpp"

std::string physicat::assets::ResolveShaderPipelinePath(const physicat::assets::ShaderPipelineType &shaderPipeline) {
    switch(shaderPipeline) {
        case physicat::assets::ShaderPipelineType::Default:
            return "default";
    }
}

std::string physicat::assets::ResolveStaticMeshPath(const physicat::assets::StaticMeshType &staticMesh) {
    switch(staticMesh) {
        case physicat::assets::StaticMeshType::Crate:
            return "assets/models/crate.obj";
    }
}

std::string physicat::assets::ResolveTexturePath(const physicat::assets::TextureType &texture) {
    switch(texture) {
        case physicat::assets::TextureType::Crate:
            return "assets/textures/crate.png";
    }
}