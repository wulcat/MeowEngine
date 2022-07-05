//
// Created by Akira Lynn on 06/07/22.
//

#include "includes.hpp"
#include "sdl_wrapper.hpp"
#include "platform.hpp"

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

std::pair<uint32_t, uint32_t> physicat::sdl::GetDisplaySize() {
    uint32_t displayWidth{0};
    uint32_t displayHeight{0};

    #ifdef __EMSCRIPTEN__
        // For Emscripten targets we will invoke some Javascript
        // to find out the dimensions of the canvas in the HTML
        // document. Note that the 'width' and 'height' attributes
        // need to be set on the <canvas /> HTML element, like so:
        // <canvas id="canvas" width="600", height="360"></canvas>

        displayWidth = static_cast<uint32_t>(EM_ASM_INT({
            return document.getElementById('canvas').width;
        }));

        displayHeight = static_cast<uint32_t>(EM_ASM_INT({
            return document.getElementById('canvas').height;
        }));
    #elif
        switch (physicat::GetCurrentPlatform()) {
            case Platform::ios:
            case Platform::android:
            {
                SDL_DisplayMode displayMode;
                // retrieves the screen size (full)
                SDL_GetDesktopDisplayMode(0, &displayMode);
                displayWidth = static_cast<uint32_t>(displayMode.w);
                displayHeight = static_cast<uint32_t>(displayMode.h);
                break;
            }
            default:
            {
                displayWidth = 640;
                displayHeight = 480;
                break;
            }

        }
    #endif

    return std::make_pair(displayWidth, displayHeight);
}

SDL_Window* physicat::sdl::CreateWindow(const uint32_t &windowFlags) {
    std::pair<uint32_t, uint32_t> displaySize{physicat::sdl::GetDisplaySize()};

    SDL_Window* window{SDL_CreateWindow(
        "Physicat Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        displaySize.first,
        displaySize.second,
        windowFlags
    )};

    if(::shouldDisplayFullScreen()) {
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    }

    return window;
}