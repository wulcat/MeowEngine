//
// Created by Akira Mujawar on 12/09/22.
//

#ifndef PHYSICAT_PERSPECTIVE_CAMERA_HPP
#define PHYSICAT_PERSPECTIVE_CAMERA_HPP

#include "../wrappers/glm_wrapper.hpp"
#include "internal_ptr.hpp"

namespace MeowEngine {
    struct PerspectiveCamera {
        PerspectiveCamera(const float& width, const float& height);

        void Configure(const glm::vec3& position, const glm::vec3& up, const glm::vec3& direction);

        //NOTE: use ref when thing is constant (in this case its dynamic so ref can be removed)
        const glm::mat4 GetProjectionMatrix() const;
        const glm::mat4 GetViewMatrix() const;
        const glm::vec3 GetPosition() const;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
} // namespace MeowEngine


#endif //PHYSICAT_PERSPECTIVE_CAMERA_HPP
