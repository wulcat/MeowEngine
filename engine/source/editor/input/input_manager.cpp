//
// Created by Akira Mujawar on 10/07/24.
//

#include <SDL_mouse.h>
#include "input_manager.hpp"

void MeowEngine::input::InputManager::ProcessInput() {
    // Update Mouse State
    mouseState = SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
}

void MeowEngine::input::InputManager::SetMouseDown() {
    isMouseDown = true;
}