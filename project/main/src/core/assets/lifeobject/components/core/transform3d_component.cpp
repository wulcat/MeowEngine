//
// Created by Akira Mujawar on 03/07/24.
//

#include "transform3d_component.hpp"

using physicat::core::component::Transform3DComponent;

Transform3DComponent::Transform3DComponent()
    : Position(glm::vec3(0,0,0))
    , Scale (glm::vec3(1,1,1))
    , Rotation (glm::vec4(0,0,0,0))
    , RotationAxis(glm::vec3(0,1,0))
    , RotationDegrees(0)
    , IdentityMatrix(glm::mat4(1.0f))
    , TransformMatrix(IdentityMatrix) {}

Transform3DComponent::Transform3DComponent(glm::vec3 position, glm::vec3 scale, glm::vec4 rotation)
    : Position(position)
    , Scale(scale)
    , Rotation(rotation)
    , RotationAxis(glm::vec3(0,1,0))
    , RotationDegrees(0)
    , IdentityMatrix(glm::mat4(1.0f))
    , TransformMatrix(IdentityMatrix) {}

Transform3DComponent::Transform3DComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotationAxis,
                                           float rotationDegrees)
    : Position(position)
    , Scale(scale)
    , Rotation(glm::vec4(0,0,0,0))
    , RotationAxis(rotationAxis)
    , RotationDegrees(rotationDegrees)
    , IdentityMatrix(glm::mat4(1.0f))
    , TransformMatrix(IdentityMatrix) {}

void Transform3DComponent::Update(const glm::mat4 &projectionViewMatrix) {
    TransformMatrix = projectionViewMatrix
                    * glm::translate(IdentityMatrix, Position)
                    * glm::rotate(IdentityMatrix, glm::radians(RotationDegrees), RotationAxis)
                    * glm::scale(IdentityMatrix, Scale);
}

void Transform3DComponent::RotateBy(const float &degrees) {
    RotationDegrees += degrees;

    if(RotationDegrees > 360.0f) {
        RotationDegrees -= 360.0f;
    }
    else if(RotationDegrees < -360.0f) {
        RotationDegrees += 360;
    }
}