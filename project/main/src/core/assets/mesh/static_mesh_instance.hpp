//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_STATIC_MESH_INSTANCE_HPP
#define PHYSICAT_STATIC_MESH_INSTANCE_HPP

#include "asset_inventory.hpp"
#include "glm_wrapper.hpp"
#include "internal_ptr.hpp"

namespace physicat {
    // In-future try ECS for this for optimization
    struct StaticMeshInstance {
        StaticMeshInstance(
            const physicat::assets::StaticMeshType& staticMeshType,
            const physicat::assets::TextureType& textureType,
            const glm::vec3& position = glm::vec3{0,0,0},
            const glm::vec3& scale = glm::vec3{0,0,0},
            const glm::vec3& rotationAxis = glm::vec3{0, 1, 0},
            const float& rotationDegrees = 0
        );

        void Update(const glm::mat4& projectionViewMatrix);
        void RotateBy(const float& degrees);
        physicat::assets::StaticMeshType GetMesh() const;
        physicat::assets::TextureType GetTexture() const;
        glm::mat4 GetTransformMatrix() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_STATIC_MESH_INSTANCE_HPP
