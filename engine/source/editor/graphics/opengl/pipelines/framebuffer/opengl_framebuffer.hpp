//
// Created by Akira Mujawar on 10/07/24.
//

#ifndef PHYSICAT_OPENGL_FRAMEBUFFER_HPP
#define PHYSICAT_OPENGL_FRAMEBUFFER_HPP

namespace physicat::graphics {
    struct OpenGLFrameBuffer {
        OpenGLFrameBuffer(const float& width, const float& height);
        ~OpenGLFrameBuffer();

        unsigned  int GetFrameTexture();
        void RescaleFrameBuffer(const float& width, const float& height);
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int frameBufferId;
        unsigned int textureId;
        unsigned int renderBufferId;
    };
}


#endif //PHYSICAT_OPENGL_FRAMEBUFFER_HPP
