//
// Created by Akira Mujawar on 03/07/24.
//

#include "transform3d_component.hpp"
#include <log.hpp>

#include "entt_reflection_wrapper.hpp"

using MeowEngine::entity::Transform3DComponent;

void MeowEngine::entity::Transform3DComponent::Reflect() {
    REGISTER_PROPERTY(Transform3DComponent, Position, MeowEngine::math::Vector3);
//    REGISTER_PROPERTY(Transform3DComponent, Scale, glm::vec3);

    REGISTER_PROPERTY(Transform3DComponent, RotationDegrees, float);

    MeowEngine::Log("Reflected", "Transform3DComponent");
}

Transform3DComponent::Transform3DComponent(const glm::mat4& inProjectionMatrix)
    : Position({0,0,0})
//    , PositionTest({1,1,1})
    , Scale (glm::vec3(1,1,1))
    , Rotation (glm::vec4(0,0,0,0))
    , RotationAxis(glm::vec3(0,1,0))
    , RotationDegrees(0)
    , IdentityMatrix(glm::mat4(1.0f))
    , TransformMatrix(IdentityMatrix) {
    CalculateTransformMatrix(inProjectionMatrix);
}

Transform3DComponent::Transform3DComponent(const glm::mat4& inProjectionMatrix, glm::vec3 position, glm::vec3 scale, glm::vec4 rotation)
    : Position({position.x, position.y, position.z})
    , Scale(scale)
    , Rotation(rotation)
    , RotationAxis(glm::vec3(0,1,0))
    , RotationDegrees(0)
    , IdentityMatrix(glm::mat4(1.0f))
    , TransformMatrix(IdentityMatrix) {
    CalculateTransformMatrix(inProjectionMatrix);
}

Transform3DComponent::Transform3DComponent(const glm::mat4& inProjectionMatrix, glm::vec3 position, glm::vec3 scale, glm::vec3 rotationAxis,
                                           float rotationDegrees)
    : Position({position.x, position.y, position.z})
    , Scale(scale)
    , Rotation(glm::vec4(0,0,0,0))
    , RotationAxis(rotationAxis)
    , RotationDegrees(rotationDegrees)
    , IdentityMatrix(glm::mat4(1.0f))
    , TransformMatrix(IdentityMatrix) {
    CalculateTransformMatrix(inProjectionMatrix);
}

void Transform3DComponent::CalculateTransformMatrix(const glm::mat4 &inProjectionMatrix) {
    TransformMatrix = inProjectionMatrix
                      * glm::translate(IdentityMatrix, glm::vec3(Position.X, Position.Y, Position.Z))
                      * glm::rotate(IdentityMatrix, glm::radians(RotationDegrees), RotationAxis)
                      * glm::scale(IdentityMatrix, Scale);
}

void Transform3DComponent::Update(const float& deltaTime) {
    float random = (float)std::rand() / RAND_MAX;
    Position.Y += 1.2f * deltaTime;

    RotateBy(1.0f * random * 0.2f);
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