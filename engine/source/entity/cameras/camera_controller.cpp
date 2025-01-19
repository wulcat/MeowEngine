//
// Created by Akira Mujawar on 06/07/24.
//

#include <log.hpp>
#include "camera_controller.hpp"

using MeowEngine::CameraController;

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

    const float MoveSpeed{5.0f};
    const float TurnSpeed{15.0f};

    float HorizontalAngle;
    float VerticalAngle;

    glm::vec3 Up;

    glm::vec3 Position;
    glm::mat4 Orientation;
    glm::vec3 ForwardDirection;

    Internal(const glm::vec3& position)
        : Identity(glm::mat4(1))
        , Up(glm::vec3{0.0f, 1.0f, 0.0f})
        , HorizontalAngle(-90.0f)
        , VerticalAngle(-10.0f)
        , Position(position)
        , Orientation(::ComputeOrientation(Identity, HorizontalAngle, Up))
        , ForwardDirection(::ComputeForwardDirection(Orientation)) {
        MeowEngine::CameraController::Internal::LookAround(0,0);
    }

    void LookAround(const float& deltaX, const float& deltaY) {
        HorizontalAngle += deltaX / TurnSpeed;
        VerticalAngle += deltaY / TurnSpeed;

        glm::vec3 YAxis {0.0f, 1.0f, 0.0f};
        glm::vec3 viewAxis {-1.0f, 0.0f, 0.0f};
        Rotate(viewAxis, HorizontalAngle, YAxis);
        Normalize(viewAxis);
        glm::vec3 aroundHorizontalAxis = Cross(YAxis, viewAxis);// YAxis.cross(viewAxis);
        Normalize(aroundHorizontalAxis);
        Rotate(viewAxis, VerticalAngle, aroundHorizontalAxis);
        Normalize(viewAxis);
        ForwardDirection = viewAxis;

        Up = Cross(ForwardDirection, aroundHorizontalAxis);
        Normalize(Up);
//        MeowEngine::Log("Logs:" , std::to_string(HorizontalAngle) + " || " + std::to_string(VerticalAngle));
    }

    void Normalize(glm::vec3& inVec) {
        float mag = sqrtf(inVec.x * inVec.x + inVec.y * inVec.y + inVec.z * inVec.z);

        inVec.x = inVec.x / mag;
        inVec.y = inVec.y / mag;
        inVec.z = inVec.z / mag;
    }

    glm::vec3 Cross(glm::vec3 a, glm::vec3 b) {
        const float x = a.y * b.z - a.z * b.y;
        const float y = a.z * b.x - a.x * b.z;
        const float z = a.x * b.y - a.y * b.x;

        return {x,y,z};
    }

    void Rotate(glm::vec3& self, float angle, const glm::vec3& axis) {
        glm::vec4 rotationQ = GetQuaternion(angle, axis);
        glm::vec4 conjugateQ = GetQuaternionConjugate(rotationQ);
        glm::vec4 t = MultiplyQuaternion(MultiplyQuaternionVec(rotationQ, self), conjugateQ);

        self.x = t.x;
        self.y = t.y;
        self.z = t.z;
    }

    glm::vec4 MultiplyQuaternion(glm::vec4 l, glm::vec4 r) {
        const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
        const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
        const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
        const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);


        return glm::vec4(x,y,z,w);
    }

    glm::vec4 MultiplyQuaternionVec(glm::vec4 q, glm::vec3 v) {
        const float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
        const float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
        const float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
        const float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);


        return glm::vec4(x,y,z,w);
    }

    glm::vec4 GetQuaternionConjugate(const glm::vec4& quat) {
        glm::vec4 q;
        q.x = -quat.x;
        q.y = -quat.y;
        q.z = -quat.z;
        q.w = quat.w;

        return q;
    }

    glm::vec4 GetQuaternion(float angle, const glm::vec3& axis) {
        float halfAngle = glm::radians(angle/2);

        float sinHalfAngle = sinf(halfAngle);
        float cosHalfAngle = cosf(halfAngle);

        glm::vec4 q;
        q.x = axis.x * sinHalfAngle;
        q.y = axis.y * sinHalfAngle;
        q.z = axis.z * sinHalfAngle;
        q.w = cosHalfAngle;

        return q;
    }

    void MoveForward(const float& delta) {
        Position -= ForwardDirection * (MoveSpeed * delta);
    }

    void MoveBackward(const float& delta) {
        Position += ForwardDirection * (MoveSpeed * delta);
    }

    void MoveUp(const float& delta) {
        Position.y += MoveSpeed * delta;
    }

    void MoveDown(const float& delta) {
        Position.y -= MoveSpeed * delta;
    }

//    void TurnLeft(const float& delta) {
//        Rotate(delta * TurnSpeed);
//    }
//
//    void TurnRight(const float& delta) {
//        Rotate(-delta * TurnSpeed);
//    }
//
//    void Rotate(const float& amount) {
//        HorizontalAngle += amount;
//
//        if (HorizontalAngle > 360.0f) {
//            HorizontalAngle -= 360.0f;
//        }
//        else if (HorizontalAngle < 0.0f) {
//            HorizontalAngle += 360.0f;
//        }
//
//        Orientation = ::ComputeOrientation(Identity, HorizontalAngle, Up);
//        ForwardDirection = ::ComputeForwardDirection(Orientation);
//    }
};

CameraController::CameraController(const glm::vec3 &position)
    : InternalPointer(MeowEngine::make_internal_ptr<Internal>(position)) {}

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

//void CameraController::TurnLeft(const float& delta) {
//    InternalPointer->TurnLeft(delta);
//}
//
//void CameraController::TurnRight(const float& delta) {
//    InternalPointer->TurnRight(delta);
//}

glm::vec3 CameraController::GetPosition() const {
    return InternalPointer->Position;
}

glm::vec3 CameraController::GetDirection() const {
    return InternalPointer->ForwardDirection;
}

glm::vec3 MeowEngine::CameraController::GetUp() const {
    return InternalPointer->Up;
}

void MeowEngine::CameraController::LookAround(const float &deltaX, const float &deltaY) {
    InternalPointer->LookAround(deltaX, deltaY);
}
