//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_ASSET_INVENTORY_HPP
#define PHYSICAT_ASSET_INVENTORY_HPP

#pragma once

#include "assets.hpp"
#include <string>

namespace physicat::assets {
    enum class ShaderPipelineType {
        Default,
        Line
    };

    enum class StaticMeshType {
        Plane,
        Cube,
        Sphere,
        Cylinder,
        Cone,
        Torus
    };

    enum class TextureType {
        Default,
        Pattern
    };

    std::string ResolveShaderPipelinePath(const physicat::assets::ShaderPipelineType& shaderPipeline);
    std::string ResolveStaticMeshPath(const physicat::assets::StaticMeshType& staticMesh);
    std::string ResolveTexturePath(const physicat::assets::TextureType& texture);
}


#endif //PHYSICAT_ASSET_INVENTORY_HPP
