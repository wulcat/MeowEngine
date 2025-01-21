//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef MEOWENGINE_OPENGL_APP_SINGLE_THREAD_HPP
#define MEOWENGINE_OPENGL_APP_SINGLE_THREAD_HPP

#include "internal_ptr.hpp"
#include "application.hpp"

namespace MeowEngine {
    struct OpenGLApplication : public MeowEngine::Application {
        OpenGLApplication();

//        void OnWindowResized() override;

        bool Input(const float& deltaTime) override;
        void FixedUpdate(const float& inFixedDeltaTime) override;
        void Update(const float& deltaTime) override;
        void Render() override;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer; // checkout pimpl pattern
    };
} // namespace MeowEngine

#endif //MEOWENGINE_OPENGL_APP_SINGLE_THREAD_HPP
