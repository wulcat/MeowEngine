//
// Created by Akira Mujawar on 04/02/25.
//

#include "opengl_collider_pipeline.hpp"
#include "transform3d_component.hpp"
#include "collider_component.hpp"

namespace MeowEngine {

    OpenGLCollidePipeline::OpenGLCollidePipeline(const GLuint &inShaderProgramID)
    : ShaderProgramID(inShaderProgramID) {
        // xyz
        float vertices[] = {
                -1, -1, -1,  1, -1, -1,  1,  1, -1,  -1,  1, -1, // Back face
                -1, -1,  1,  1, -1,  1,  1,  1,  1,  -1,  1,  1, // Front face
                -1, -1, -1, -1,  1, -1, -1,  1,  1,  -1, -1,  1, // Left face
                1, -1, -1,  1,  1, -1,  1,  1,  1,   1, -1,  1, // Right face
                -1,  1, -1,  1,  1, -1,  1,  1,  1,  -1,  1,  1, // Top face
                -1, -1, -1,  1, -1, -1,  1, -1,  1,  -1, -1,  1  // Bottom face
        };
        // trinagle
        unsigned int indices[] = {
                0, 1, 2,  2, 3, 0,  // Back face
                4, 5, 6,  6, 7, 4,  // Front face
                8, 9, 10, 10, 11, 8, // Left face
                12,13,14, 14,15,12, // Right face
                16,17,18, 18,19,16, // Top face
                20,21,22, 22,23,20  // Bottom face
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // dynamic instances
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        for(int i = 0 ; i < 4; i++) {
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
            glEnableVertexAttribArray(1 + i);

            glVertexAttribDivisor(1 + i, 1);
        }

        glBindVertexArray(0);
    }

    OpenGLCollidePipeline::~OpenGLCollidePipeline() noexcept {}

    void OpenGLCollidePipeline::Render(MeowEngine::PerspectiveCamera* cameraObject, entt::registry& registry) {
        // Since physics runs on separate thread,
        // we access only the transform data from render buffer
        // & using its property component data we draw the collider
        std::vector<glm::mat4> colliders;
        for(auto &&[entity, transform, collider]: registry.view<entity::Transform3DComponent, entity::ColliderComponent>().each()) {
            colliders.push_back(transform.TransformMatrix);
        }

        glUseProgram(ShaderProgramID);
        glDisable(GL_CULL_FACE);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_view"), 1, GL_FALSE, &cameraObject->GetViewMatrix()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "u_projection"), 1, GL_FALSE, &cameraObject->GetProjectionMatrix()[0][0]);
#ifndef USING_GLES
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
        glBindVertexArray(VAO);

        // NOTE: need to recheck this as layout in shader doesn't work for web builds
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        if(!colliders.empty()) {
            glBufferData(GL_ARRAY_BUFFER, colliders.size() * sizeof(glm::mat4), colliders.data(), GL_DYNAMIC_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, colliders.size());
#ifndef USING_GLES
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
        glBindVertexArray(0);
    }
} // MeowEngine