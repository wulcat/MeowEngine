//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_ASSET_INVENTORY_HPP
#define PHYSICAT_ASSET_INVENTORY_HPP

#pragma once

#include "assets.hpp"
#include <string>

namespace MeowEngine::assets {
    enum class ShaderPipelineType {
        Default,
        Line,
        Grid
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

    std::string ResolveShaderPipelinePath(const MeowEngine::assets::ShaderPipelineType& shaderPipeline);
    std::string ResolveStaticMeshPath(const MeowEngine::assets::StaticMeshType& staticMesh);
    std::string ResolveTexturePath(const MeowEngine::assets::TextureType& texture);
}


#endif //PHYSICAT_ASSET_INVENTORY_HPP
