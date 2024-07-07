//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_OPENGL_PIPELINE_BASE_HPP
#define PHYSICAT_OPENGL_PIPELINE_BASE_HPP

#include "log.hpp"

namespace physicat {
    struct OpenGLAssetManager;

    namespace pipeline {
        struct OpenGLPipelineBase {
            OpenGLPipelineBase() = default;
            virtual ~OpenGLPipelineBase() = default;
        };
    }
}

#endif //PHYSICAT_OPENGL_PIPELINE_BASE_HPP
