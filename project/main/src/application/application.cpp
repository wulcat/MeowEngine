//
// Created by Akira Lynn on 06/07/22.
//

#include "../core/sdl_wrapper.hpp"
#include "application.hpp"

using physicat::Application;

namespace {
    #ifdef EMSCRIPTEN
        // Free Function in an anonymous namespace private to this file
        void EmscriptenLoop(physicat::Application* engine) {
            engine->Update();
        }
    #endif
} // namespace



void physicat::Application::Begin() {
    #ifdef __EMSCRIPTEN__
        //  emscripten_set_main_loop(emscriptenLoop, 60, 1);
        emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
    #else
        while (Update())
        {
            // Just waiting for the main loop to end.
        }
    #endif
}

bool physicat::Application::Update() {
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
    Render();

    // Returning true means we want to keep looping.
    return true;
}
