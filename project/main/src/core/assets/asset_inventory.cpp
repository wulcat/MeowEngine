//
// Created by Akira Mujawar on 21/06/24.
//

#include "asset_inventory.hpp"

using physicat::assets::StaticMeshType;
using physicat::assets::TextureType;

std::string physicat::assets::ResolveShaderPipelinePath(const physicat::assets::ShaderPipelineType &shaderPipeline) {
    switch(shaderPipeline) {
        case physicat::assets::ShaderPipelineType::Default:
            return "default";
    }
}

std::string physicat::assets::ResolveStaticMeshPath(const physicat::assets::StaticMeshType &staticMesh) {
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

std::string physicat::assets::ResolveTexturePath(const physicat::assets::TextureType &texture) {
    switch(texture) {
        case TextureType::Default:
            return "assets/textures/default.png";
        case TextureType::Pattern:
            return "assets/textures/pattern.jpg";
    }
}