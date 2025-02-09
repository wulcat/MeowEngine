//
// Created by Akira Mujawar on 04/02/25.
//

#ifndef MEOWENGINE_OPENGL_COLLIDER_PIPELINE_HPP
#define MEOWENGINE_OPENGL_COLLIDER_PIPELINE_HPP

#include "graphics_wrapper.hpp"

#include "opengl_pipeline_base.hpp"
#include "perspective_camera.hpp"
#include "entt_wrapper.hpp"

namespace MeowEngine {

    struct OpenGLCollidePipeline : public MeowEngine::pipeline::OpenGLPipelineBase {
        OpenGLCollidePipeline(const GLuint& inShaderProgramID);
        ~OpenGLCollidePipeline() override;

    public:
        void Render(MeowEngine::PerspectiveCamera* cameraObject, entt::registry& registry);

    private:
        const GLuint ShaderProgramID;

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int instanceVBO;
    };

} // MeowEngine

#endif //MEOWENGINE_OPENGL_COLLIDER_PIPELINE_HPP
