//
// Created by Akira Lynn on 12/09/22.
//

#include "perspective_camera.hpp"
#include "log.hpp"
using physicat::PerspectiveCamera;

namespace {
    glm::mat4 CreateProjectionMatrix(const float& width, const float& height) {
        return glm::perspective(glm::radians(60.0f), width / height, 0.01f, 100.0f);
    }

    glm::mat4 CreateViewMatrix(glm::vec3 position) {
        glm::vec3 target {0.0f, 0.0f, 0.0f}; // at what point the camera should look at
        glm::vec3 up {0.0f, 1.0f, 0.0f}; // the direction of the camera (pointing upward)

        return glm::lookAt(position, target, up);
    }
} // namespace

struct PerspectiveCamera::Internal {
    glm::vec3 Position;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;

    Internal(const float& width, const float& height)
        : ProjectionMatrix(CreateProjectionMatrix(width, height))
        , Position(0.0f, 2.0f , -4.0f)
        , ViewMatrix(CreateViewMatrix(Position)) {
//        physicat::Log("sdfs",)
    }
};

physicat::PerspectiveCamera::PerspectiveCamera(const float &width, const float &height)
    :  InternalPointer(make_internal_ptr<Internal>(width, height)) {}

const glm::mat4 &physicat::PerspectiveCamera::GetProjectionMatrix() const {
    return InternalPointer->ProjectionMatrix;
}

const glm::mat4 &physicat::PerspectiveCamera::GetViewMatrix() const {
    return InternalPointer->ViewMatrix;
}

const glm::vec3 &physicat::PerspectiveCamera::GetPosition() const {
    return InternalPointer->Position;
}
