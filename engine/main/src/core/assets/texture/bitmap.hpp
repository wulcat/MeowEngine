//
// Created by Akira Mujawar on 19/06/24.
//

#ifndef PHYSICAT_BITMAP_HPP
#define PHYSICAT_BITMAP_HPP

#pragma once

#include "src/utils/internal_ptr.hpp"
#include "sdl_wrapper.hpp"

namespace physicat {
    struct Bitmap {
        Bitmap(SDL_Surface* surface);

        uint16_t GetWidth() const;

        uint16_t GetHeight() const;

        // Gives memory pointer to the data of bitmap. Required for OpenGL/Vulkan
        void* GetPixelData() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_BITMAP_HPP
