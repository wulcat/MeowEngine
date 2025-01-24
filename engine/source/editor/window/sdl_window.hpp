//
// Created by Akira Mujawar on 22/10/24.
//

#ifndef MEOWENGINE_WINDOW_HPP
#define MEOWENGINE_WINDOW_HPP

#include "sdl_wrapper.hpp"
#include "log.hpp"

namespace MeowEngine {
    struct SDLWindow {
        SDLWindow()
        : window(MeowEngine::sdl::CreateWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)),
          context(MeowEngine::sdl::CreateContext(window))
        {}

        ~SDLWindow()
        {
            MeowEngine::Log("Window", "Destroyed");
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
        }

        void SwapWindow() const {
            SDL_GL_SwapWindow(window);
        }

        SDL_Window* window;
        SDL_GLContext context;
    };
}


#endif //MEOWENGINE_WINDOW_HPP
