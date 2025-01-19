//
// Created by Akira Mujawar on 10/07/24.
//

#ifndef PHYSICAT_INPUT_MANAGER_HPP
#define PHYSICAT_INPUT_MANAGER_HPP

#include <cstdint>

namespace MeowEngine::input {
    struct InputManager {
    public:
        void ProcessInput();
        void SetMouseDown();

        bool isActive;
        uint32_t mouseState;
        int mouseDeltaX;
        int mouseDeltaY;
        bool isMouseDown;
    };
}


#endif //PHYSICAT_INPUT_MANAGER_HPP
