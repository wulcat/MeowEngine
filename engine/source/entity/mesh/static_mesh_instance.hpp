//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_STATIC_MESH_INSTANCE_HPP
#define PHYSICAT_STATIC_MESH_INSTANCE_HPP

#include "asset_inventory.hpp"
//#include "glm_wrapper.hpp"
#include "internal_ptr.hpp"

namespace MeowEngine {
    // In-future try ECS for this for optimization
    struct StaticMeshInstance {
        StaticMeshInstance(
            const MeowEngine::assets::StaticMeshType& staticMeshType,
            const MeowEngine::assets::TextureType& textureType
        );

//        void Update(const glm::mat4& projectionViewMatrix);
//        void RotateBy(const float& degrees);
        MeowEngine::assets::StaticMeshType GetMesh() const;
        MeowEngine::assets::TextureType GetTexture() const;
//        glm::mat4 GetTransformMatrix() const;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_STATIC_MESH_INSTANCE_HPP
