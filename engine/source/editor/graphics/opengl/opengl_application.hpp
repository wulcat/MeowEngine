//
// Created by Akira Lynn on 06/07/22.
//

#ifndef PHYSICAT_OPENGL_APPLICATION_HPP
#define PHYSICAT_OPENGL_APPLICATION_HPP

#include "internal_ptr.hpp"
#include "application.hpp"

namespace physicat {
    struct OpenGLApplication : public physicat::Application {
        OpenGLApplication();

//        void OnWindowResized() override;

        bool Input(const float& deltaTime) override;
        void Update(const float& deltaTime) override;
        void Render() override;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer; // checkout pimpl pattern
    };
} // namespace physicat

#endif //PHYSICAT_OPENGL_APPLICATION_HPP
