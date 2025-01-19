//
// Created by Akira Mujawar on 20/06/24.
//

#ifndef PHYSICAT_OPENGL_TEXTURE_HPP
#define PHYSICAT_OPENGL_TEXTURE_HPP

#pragma once

#include "bitmap.hpp"
#include "internal_ptr.hpp"

namespace MeowEngine {
    struct OpenGLTexture {
        OpenGLTexture(const MeowEngine::Bitmap& bitmap);

        // need to call whenever we want the texture to be applied to the object being rendered
        void Bind() const;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_OPENGL_TEXTURE_HPP
