//
// Created by Akira Mujawar on 06/07/24.
//

#include "camera_controller.hpp"

using physicat::CameraController;

namespace {
    glm::mat4 ComputeOrientation(const glm::mat4& identity, const float& rotationDegrees, const glm::vec3& up) {
        return glm::rotate(identity, glm::radians(rotationDegrees), up);
    }

    glm::vec3 ComputeForwardDirection(const glm::mat4& orientation) {
        return glm::normalize(orientation * glm::vec4(0,0,1,0));
    }
}

struct CameraController::Internal {
    const glm::mat4 Identity;
    const glm::vec3 Up;
    const float MoveSpeed{5.0f};
    const float TurnSpeed{120.0f};

    float RotationDegrees;
    glm::vec3 Position;
    glm::mat4 Orientation;
    glm::vec3 ForwardDirection;

    Internal(const glm::vec3& position)
        : Identity(glm::mat4(1))
        , Up(glm::vec3{0.0f, 1.0f, 0.0f})
        , RotationDegrees(0.0f)
        , Position(position)
        , Orientation(::ComputeOrientation(Identity, RotationDegrees, Up))
        , ForwardDirection(::ComputeForwardDirection(Orientation)) {}

    void MoveForward(const float& delta) {
        Position += ForwardDirection * (MoveSpeed * delta);
    }

    void MoveBackward(const float& delta) {
        Position -= ForwardDirection * (MoveSpeed * delta);
    }

    void MoveUp(const float& delta) {
        Position.y += MoveSpeed * delta;
    }

    void MoveDown(const float& delta) {
        Position.y -= MoveSpeed * delta;
    }

    void TurnLeft(const float& delta) {
        Rotate(delta * TurnSpeed);
    }

    void TurnRight(const float& delta) {
        Rotate(-delta * TurnSpeed);
    }

    void Rotate(const float& amount) {
        RotationDegrees += amount;

        if (RotationDegrees > 360.0f) {
            RotationDegrees -= 360.0f;
        }
        else if (RotationDegrees < 0.0f) {
            RotationDegrees += 360.0f;
        }

        Orientation = ::ComputeOrientation(Identity, RotationDegrees, Up);
        ForwardDirection = ::ComputeForwardDirection(Orientation);
    }
};

CameraController::CameraController(const glm::vec3 &position)
    : InternalPointer(physicat::make_internal_ptr<Internal>(position)) {}

void CameraController::MoveForward(const float& delta) {
    InternalPointer->MoveForward(delta);
}

void CameraController::MoveBackward(const float& delta) {
    InternalPointer->MoveBackward(delta);
}

void CameraController::MoveUp(const float& delta) {
    InternalPointer->MoveUp(delta);
}

void CameraController::MoveDown(const float& delta) {
    InternalPointer->MoveDown(delta);
}

void CameraController::TurnLeft(const float& delta) {
    InternalPointer->TurnLeft(delta);
}

void CameraController::TurnRight(const float& delta) {
    InternalPointer->TurnRight(delta);
}

glm::vec3 CameraController::GetPosition() const {
    return InternalPointer->Position;
}

glm::vec3 CameraController::GetDirection() const {
    return InternalPointer->ForwardDirection;
}