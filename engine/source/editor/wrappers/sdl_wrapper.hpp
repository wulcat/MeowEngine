//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef PHYSICAT_SDL_WRAPPER_HPP
#define PHYSICAT_SDL_WRAPPER_HPP

#pragma once

#include <SDL.h>
#include <utility>

#include "window_size.hpp"

namespace MeowEngine::sdl {
    MeowEngine::WindowSize GetInitialWindowSize();
    MeowEngine::WindowSize GetWindowSize(SDL_Window* window);
    SDL_Window* CreateWindow(const uint32_t& windowFlags);
    SDL_GLContext CreateContext(SDL_Window* window);
}

#endif //PHYSICAT_SDL_WRAPPER_HPP
