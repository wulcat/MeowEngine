//
// Created by Akira Mujawar on 06/07/22.
//

#include "../includes.hpp"
#include "sdl_wrapper.hpp"
#include "platform.hpp"
#include "log.hpp"

namespace // wth is this ._. ?
{
    bool shouldDisplayFullScreen(){
        switch (physicat::GetCurrentPlatform()) {
            case physicat::Platform::ios:
                return true;
            case physicat::Platform::android:
                return true;
            case physicat::Platform::windows:
                return false;
            case physicat::Platform::mac:
                return false;
            case physicat::Platform::emscripten:
                return false;
        }
    }
} // namespace

#ifdef __EMSCRIPTEN__
    physicat::WindowSize GetEmscriptenCanvasSize() {
        // For Emscripten targets we will invoke some Javascript
        // to find out the dimensions of the canvas in the HTML
        // document. Note that the 'width' and 'height' attributes
        // need to be set on the <canvas /> HTML element, like so:
        // <canvas id="canvas" width="600", height="360"></canvas>

        uint32_t width{static_cast<uint32_t>(EM_ASM_INT({
            return document.getElementById('canvas').width;
        }))};

        uint32_t height{static_cast<uint32_t>(EM_ASM_INT({
            return document.getElementById('canvas').height;
        }))};

        return physicat::WindowSize{width, height};
    };
#endif

physicat::WindowSize physicat::sdl::GetInitialWindowSize() {
#ifdef __EMSCRIPTEN__
    return ::GetEmscriptenCanvasSize();
#else
    const physicat::Platform platform{physicat::GetCurrentPlatform()};

    if (platform == physicat::Platform::ios || platform == physicat::Platform::android)
    {
        // For mobile platforms we will fetch the full screen size.
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        return physicat::WindowSize{static_cast<uint32_t>(displayMode.w), static_cast<uint32_t>(displayMode.h)};
    }

    // For other platforms we'll just show a fixed size window.
    return physicat::WindowSize{1000, 500};
#endif
}

physicat::WindowSize physicat::sdl::GetWindowSize(SDL_Window* window) {
    uint32_t displayWidth{0};
    uint32_t displayHeight{0};

    #ifdef __EMSCRIPTEN__
        return ::GetEmscriptenCanvasSize();
    #else
        int width {0};
        int height {0};

        SDL_GetWindowSize(window, &width, &height);
        return physicat::WindowSize{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    #endif
}

SDL_Window* physicat::sdl::CreateWindow(const uint32_t &windowFlags) {
    physicat::WindowSize windowSize{physicat::sdl::GetInitialWindowSize()};


#ifdef __EMSCRIPTEN__
        // GL 3.0 + GLSL 130
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    // Set the attributes for SDL (here we are supporting from 1.0 to 4.0 version of shader)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
//    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
//    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    SDL_Window* window{SDL_CreateWindow(
        "Physicat",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowSize.Width,
        windowSize.Height,
        windowFlags
    )};

    if(::shouldDisplayFullScreen()) {
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    }

    return window;
}