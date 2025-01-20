//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef MEOWENGINE_OPENGL_PIPELINE_BASE_HPP
#define MEOWENGINE_OPENGL_PIPELINE_BASE_HPP

#include "log.hpp"

namespace MeowEngine {
    struct OpenGLAssetManager;

    namespace pipeline {
        struct OpenGLPipelineBase {
            OpenGLPipelineBase() = default;
            virtual ~OpenGLPipelineBase() = default;
        };
    }
}

#endif //MEOWENGINE_OPENGL_PIPELINE_BASE_HPP
