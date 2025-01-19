//
// Created by Akira Mujawar on 20/06/24.
//

#include "opengl_texture.hpp"
#include "graphics_wrapper.hpp"

using MeowEngine::OpenGLTexture;

namespace {
    GLuint CreateTexture(const MeowEngine::Bitmap& bitmap) {
        GLuint TextureID;

        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            bitmap.GetWidth(),
            bitmap.GetHeight(),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bitmap.GetPixelData()
        );

        return TextureID;
    }
} // namespace

struct OpenGLTexture::Internal {
    const GLuint TextureID;

    Internal(const MeowEngine::Bitmap& bitmap)
        : TextureID(::CreateTexture(bitmap)) {}

    ~Internal() {
        glDeleteTextures(1, &TextureID);
    }
};

OpenGLTexture::OpenGLTexture(const MeowEngine::Bitmap &bitmap)
    : InternalPointer(MeowEngine::make_internal_ptr<Internal>(bitmap)){}

void OpenGLTexture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, InternalPointer->TextureID);
}