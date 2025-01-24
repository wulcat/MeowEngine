//
// Created by Akira Mujawar on 21/06/24.
//

#include "asset_inventory.hpp"

using MeowEngine::assets::StaticMeshType;
using MeowEngine::assets::TextureType;

std::string MeowEngine::assets::ResolveShaderPipelinePath(const MeowEngine::assets::ShaderPipelineType &shaderPipeline) {
    switch(shaderPipeline) {
        case MeowEngine::assets::ShaderPipelineType::Default:
            return "default";
        case MeowEngine::assets::ShaderPipelineType::Line:
            return "line";
        case MeowEngine::assets::ShaderPipelineType::Grid:
            return "grid";
        case MeowEngine::assets::ShaderPipelineType::Sky:
            return "sky";
    }
}

std::string MeowEngine::assets::ResolveStaticMeshPath(const MeowEngine::assets::StaticMeshType &staticMesh) {
    switch(staticMesh) {
        case StaticMeshType::Plane:
            return "assets/models/plane.obj";
        case StaticMeshType::Cube:
            return "assets/models/cube.obj";
        case StaticMeshType::Sphere:
            return "assets/models/sphere.obj";
        case StaticMeshType::Cylinder:
            return "assets/models/cylinder.obj";
        case StaticMeshType::Cone:
            return "assets/models/cone.obj";
        case StaticMeshType::Torus:
            return "assets/models/torus.obj";
    }
}

std::string MeowEngine::assets::ResolveTexturePath(const MeowEngine::assets::TextureType &texture) {
    switch(texture) {
        case TextureType::Default:
            return "assets/textures/default.png";
        case TextureType::Pattern:
            return "assets/textures/pattern.jpg";
    }
}