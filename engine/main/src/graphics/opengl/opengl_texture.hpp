//
// Created by Akira Mujawar on 20/06/24.
//

#ifndef PHYSICAT_OPENGL_TEXTURE_HPP
#define PHYSICAT_OPENGL_TEXTURE_HPP

#pragma once

#include "bitmap.hpp"
#include "internal_ptr.hpp"

namespace physicat {
    struct OpenGLTexture {
        OpenGLTexture(const physicat::Bitmap& bitmap);

        // need to call whenever we want the texture to be applied to the object being rendered
        void Bind() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}

#endif //PHYSICAT_OPENGL_TEXTURE_HPP
