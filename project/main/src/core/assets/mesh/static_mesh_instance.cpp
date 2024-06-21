//
// Created by Akira Mujawar on 21/06/24.
//

#include "static_mesh_instance.hpp"

using physicat::StaticMeshInstance;

struct StaticMeshInstance::Internal {
    const physicat::assets::StaticMeshType MeshType;
    const physicat::assets::TextureType TextureType;
    const glm::mat4 Identity;

    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 RotationAxis;
    float RotationDegrees;
    glm::mat4 TransformMatrix;

    Internal(
        const physicat::assets::StaticMeshType& meshType,
        const physicat::assets::TextureType& textureType,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& rotationAxis,
        const float& rotationDegrees
    )
    : MeshType(meshType)
    , TextureType(textureType)
    , Identity(glm::mat4{1.0f})
    , Position(position)
    , Scale(scale)
    , RotationAxis(rotationAxis)
    , RotationDegrees(rotationDegrees)
    , TransformMatrix(Identity) {}

    void Update(const glm::mat4& projectionViewMatrix) {
        //  https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices
        TransformMatrix = projectionViewMatrix
                          * glm::translate(Identity, Position)
                          * glm::rotate(Identity, glm::radians(RotationDegrees), RotationAxis)
                          * glm::scale(Identity, Scale);
    }

    void RotateBy(const float& degrees) {
        RotationDegrees += degrees;

        if(RotationDegrees > 360.0f) {
            RotationDegrees -= 360.0f;
        }
        else if(RotationDegrees < -360.0f) {
            RotationDegrees += 360;
        }
    }
};

StaticMeshInstance::StaticMeshInstance(
    const physicat::assets::StaticMeshType& staticMeshType,
    const physicat::assets::TextureType& textureType,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& rotationAxis,
    const float &rotationDegrees)
    : InternalPointer(physicat::make_internal_ptr<Internal>(
        staticMeshType,
        textureType,
        position,
        scale,
        rotationAxis,
        rotationDegrees
    )) {}

void StaticMeshInstance::Update(const glm::mat4 &projectionViewMatrix) {
    InternalPointer->Update(projectionViewMatrix);
}

void StaticMeshInstance::RotateBy(const float &degrees) {
    InternalPointer->RotateBy(degrees);
}

physicat::assets::StaticMeshType StaticMeshInstance::GetMesh() const {
    return InternalPointer->MeshType;
}

physicat::assets::TextureType StaticMeshInstance::GetTexture() const {
    return InternalPointer->TextureType;
}

glm::mat4 StaticMeshInstance::GetTransformMatrix() const {
    return InternalPointer->TransformMatrix;
}








