//
// Created by Akira Lynn on 06/07/22.
//

//#ifndef VULKAN_ENGINE_SDL_WRAPPER_HPP
//#define VULKAN_ENGINE_SDL_WRAPPER_HPP

#pragma once

#include <SDL.h>
#include <utility>

#include "window_size.hpp"

namespace physicat::sdl {
    physicat::WindowSize GetInitialWindowSize();
    physicat::WindowSize GetWindowSize(SDL_Window* window);
    SDL_Window* CreateWindow(const uint32_t& windowFlags);
}

//#endif //VULKAN_ENGINE_SDL_WRAPPER_HPP
