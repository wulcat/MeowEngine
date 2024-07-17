//
// Created by Akira Mujawar on 10/07/24.
//

#ifndef PHYSICAT_INPUT_MANAGER_HPP
#define PHYSICAT_INPUT_MANAGER_HPP

#include <cstdint>

namespace physicat::input {
    struct InputManager {
    public:
        void ProcessInput();

        bool isActive;
        uint32_t mouseState;
        int mouseDeltaX;
        int mouseDeltaY;
    };
}


#endif //PHYSICAT_INPUT_MANAGER_HPP
