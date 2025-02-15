//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef MEOWENGINE_OPENGL_APP_SINGLE_THREAD_HPP
#define MEOWENGINE_OPENGL_APP_SINGLE_THREAD_HPP

#include "internal_ptr.hpp"
#include "application.hpp"

namespace MeowEngine {
    struct OpenGLAppSingleThread : public MeowEngine::Application {
        OpenGLAppSingleThread();

//        void OnWindowResized() override;

        bool Input(const float& deltaTime) ;
        void FixedUpdate(const float& inFixedDeltaTime) ;
        void Update(const float& deltaTime) ;
        void Render() ;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer; // checkout pimpl pattern
    };
} // namespace MeowEngine

#endif //MEOWENGINE_OPENGL_APP_SINGLE_THREAD_HPP
