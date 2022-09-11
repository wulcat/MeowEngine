//
// Created by Akira Lynn on 11/09/22.
//

#ifndef PHYSICAT_OPENGL_PIPELINE_HPP
#define PHYSICAT_OPENGL_PIPELINE_HPP

#include "../../core/wrappers/graphics_wrapper.hpp"
#include "../../core/internal_ptr.hpp"

namespace physicat {
    struct OpenGLPipeline {
        explicit OpenGLPipeline(const std::string& shaderName);

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat


#endif //PHYSICAT_OPENGL_PIPELINE_HPP
