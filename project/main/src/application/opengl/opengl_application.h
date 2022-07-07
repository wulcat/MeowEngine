//
// Created by Akira Lynn on 06/07/22.
//

#ifndef PHYSICAT_OPENGL_APPLICATION_H
#define PHYSICAT_OPENGL_APPLICATION_H

#include "../../core/internal_ptr.hpp"
#include "../application.hpp"

namespace physicat {
    struct OpenGLApplication : public physicat::Application {
        OpenGLApplication();
        void Render(Scene* scene) override;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> internal; // checkout pimpl pattern
    };
} // namespace physicat

#endif //PHYSICAT_OPENGL_APPLICATION_H
