//
// Created by Akira Mujawar on 10/07/24.
//

#include "opengl_framebuffer.hpp"

#include "graphics_wrapper.hpp"
#include "log.hpp"

using physicat::graphics::OpenGLFrameBuffer;

OpenGLFrameBuffer::OpenGLFrameBuffer(const float &width, const float &height)
    : frameBufferId(0)
    , textureId(0)
    , renderBufferId(0) {

    glGenFramebuffers(1, &frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    glGenRenderbuffers(1, &renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        physicat::Log("ERROR::FRAMEBUFFER::", "Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    glDeleteFramebuffers(1, &frameBufferId);
    glDeleteTextures(1, &textureId);
    glDeleteRenderbuffers(1, &renderBufferId);
}

unsigned int OpenGLFrameBuffer::GetFrameTexture() {
    return textureId;
}

void OpenGLFrameBuffer::RescaleFrameBuffer(const float &width, const float &height) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void OpenGLFrameBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

void OpenGLFrameBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
