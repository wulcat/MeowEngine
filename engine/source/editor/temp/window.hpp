//
// Created by Akira Mujawar on 22/10/24.
//

#ifndef PHYSICAT_WINDOW_HPP
#define PHYSICAT_WINDOW_HPP

#include "sdl_wrapper.hpp"
#include "log.hpp"

namespace physicat {
    struct Window {
        SDL_Window* window;
        SDL_GLContext context;

        Window()
        : window(physicat::sdl::CreateWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)),
          context(physicat::sdl::CreateContext(window))
        {}

        ~Window()
        {
            physicat::Log("Window", "Destroyed");
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
        }
    };
}


#endif //PHYSICAT_WINDOW_HPP
