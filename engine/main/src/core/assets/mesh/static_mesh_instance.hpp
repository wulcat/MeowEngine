//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_STATIC_MESH_INSTANCE_HPP
#define PHYSICAT_STATIC_MESH_INSTANCE_HPP

#include "asset_inventory.hpp"
//#include "glm_wrapper.hpp"
#include "src/utils/internal_ptr.hpp"

namespace physicat {
    // In-future try ECS for this for optimization
    struct StaticMeshInstance {
        StaticMeshInstance(
            const physicat::assets::StaticMeshType& staticMeshType,
            const physicat::assets::TextureType& textureType
        );

//        void Update(const glm::mat4& projectionViewMatrix);
//        void RotateBy(const float& degrees);
        physicat::assets::StaticMeshType GetMesh() const;
        physicat::assets::TextureType GetTexture() const;
//        glm::mat4 GetTransformMatrix() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_STATIC_MESH_INSTANCE_HPP
