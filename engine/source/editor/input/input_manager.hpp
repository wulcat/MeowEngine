//
// Created by Akira Mujawar on 10/07/24.
//

#ifndef MEOWENGINE_INPUT_MANAGER_HPP
#define MEOWENGINE_INPUT_MANAGER_HPP

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


#endif //MEOWENGINE_INPUT_MANAGER_HPP
