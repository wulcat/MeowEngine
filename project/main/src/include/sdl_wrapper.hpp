//
// Created by Akira Lynn on 06/07/22.
//

//#ifndef VULKAN_ENGINE_SDL_WRAPPER_HPP
//#define VULKAN_ENGINE_SDL_WRAPPER_HPP

#pragma once

#include <SDL.h>
#include <utility>

namespace physicat::sdl {
    std::pair<uint32_t, uint32_t> GetDisplaySize();

    SDL_Window* CreateWindow(const uint32_t& windowFlags);
}

//#endif //VULKAN_ENGINE_SDL_WRAPPER_HPP
