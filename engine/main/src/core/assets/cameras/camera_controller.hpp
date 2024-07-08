//
// Created by Akira Mujawar on 06/07/24.
//

#ifndef PHYSICAT_CAMERA_CONTROLLER_HPP
#define PHYSICAT_CAMERA_CONTROLLER_HPP

#include "glm_wrapper.hpp"
#include "src/utils/internal_ptr.hpp"

namespace physicat {
    struct CameraController {
    public:
        CameraController(const glm::vec3& position);

        void LookAround(const float& deltaX, const float& deltaY);
        void MoveForward(const float& delta);
        void MoveBackward(const float& delta);
        void MoveUp(const float& delta);
        void MoveDown(const float& delta);
//        void TurnLeft(const float& delta);
//        void TurnRight(const float& delta);

        glm::vec3 GetPosition() const;
        glm::vec3 GetUp() const;
        glm::vec3 GetDirection() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_CAMERA_CONTROLLER_HPP
