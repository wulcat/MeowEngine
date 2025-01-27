//
// Created by Akira Mujawar on 19/06/24.
//

#include "bitmap.hpp"

using MeowEngine::Bitmap;

struct Bitmap::Internal {
    SDL_Surface* surface;

    Internal(SDL_Surface* surface) : surface(surface) {}

    ~Internal() {
        SDL_FreeSurface(surface);
    }
};

Bitmap::Bitmap(SDL_Surface *surface) : InternalPointer(MeowEngine::make_internal_ptr<Internal>(surface)) {}

uint16_t Bitmap::GetWidth() const {
    return static_cast<uint16_t>(InternalPointer->surface->w);
}

uint16_t Bitmap::GetHeight() const {
    return static_cast<uint16_t>(InternalPointer->surface->h);
}

// REMEMBER: void* is a generic pointer type and can be casted to other pointer without explicit cast
void* Bitmap::GetPixelData() const {
    return InternalPointer->surface->pixels;
}