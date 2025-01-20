//
// Created by Akira Mujawar on 19/06/24.
//

#ifndef MEOWENGINE_BITMAP_HPP
#define MEOWENGINE_BITMAP_HPP

#pragma once

#include "internal_ptr.hpp"
#include "sdl_wrapper.hpp"

namespace MeowEngine {
    struct Bitmap {
        Bitmap(SDL_Surface* surface);

        uint16_t GetWidth() const;

        uint16_t GetHeight() const;

        // Gives memory pointer to the data of bitmap. Required for OpenGL/Vulkan
        void* GetPixelData() const;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}


#endif //MEOWENGINE_BITMAP_HPP
