//
// Created by Akira Lynn on 12/09/22.
//

#ifndef PHYSICAT_PERSPECTIVE_CAMERA_HPP
#define PHYSICAT_PERSPECTIVE_CAMERA_HPP

#include "../wrappers/glm_wrapper.hpp"
#include "../internal_ptr.hpp"

namespace physicat {
    struct PerspectiveCamera {
        PerspectiveCamera(const float& width, const float& height);

        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;
        const glm::vec3& GetPosition() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat


#endif //PHYSICAT_PERSPECTIVE_CAMERA_HPP
