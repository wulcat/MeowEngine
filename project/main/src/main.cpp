//
// Created by Akira Lynn on 05/07/22.
//

#include <iostream>
#include <SDL.h>

#define GL_SILENCE_DEPRECATION
//#include <OpenGL/gl3.h>
#pragma once

#if defined(__EMSCRIPTEN__)
#include <GLES2/gl2.h>
#elif __APPLE__
#define GL_SILENCE_DEPRECATION
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#include <OpenGLES/ES2/gl.h>
#else
#include <OpenGL/gl3.h>
#endif
#elif __ANDROID__
#include <GLES2/gl2.h>
#endif

void render(SDL_Window* window, const SDL_GLContext& context) {
    SDL_GL_MakeCurrent(window, context);

    glClearColor(0.3f, 0.7f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}

bool loop(SDL_Window* window, const SDL_GLContext& context) {
    SDL_Event event;

    // Each loop we will process any events that are waiting for us.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // If we get a quit signal, we will return that we don't want to keep looping.
            case SDL_QUIT:
                return false;

            case SDL_KEYDOWN:
                // If we get a key down event for the ESC key, we also don't want to keep looping.
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return false;
                }
                break;
            default:
                break;
        }
    }

    // Perform our rendering for this frame, normally you would also perform
    // any updates to your world as well here.
    render(window, context);

    // Returning true means we want to keep looping.
    return true;
}

void run() {
    uint32_t width{640};
    uint32_t height{640};

    // Create a new SDL window based on OpenGL.
    SDL_Window* window{SDL_CreateWindow(
            "A Simple Triangle",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)};

    // Obtain an OpenGL context based on our window.
    SDL_GLContext context{SDL_GL_CreateContext(window)};

    // Setup some basic global OpenGL state.
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, width, height);

    while (loop(window, context))
    {
        // Just waiting for the main loop to end.
    }

    // Clean up after ourselves.
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

int main(int, char* []) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        std::cout << "Successfully initialized SDL!" << std::endl;

        run();

        SDL_Quit();
    }
    else {
        std::cout << "Failed to initialized SDL" << std::endl;
    }


    return 0;
}