//
// Created by Akira Lynn on 12/09/22.
//

#ifndef PHYSICAT_OPENGL_MESH_HPP
#define PHYSICAT_OPENGL_MESH_HPP

#include "graphics_wrapper.hpp"
#include "mesh.hpp"
#include "internal_ptr.hpp"

namespace MeowEngine {
    struct OpenGLMesh {
        explicit OpenGLMesh(const MeowEngine::Mesh& mesh);

        const GLuint & GetVertexArrayId() const;
        const GLuint& GetVertexBufferId() const;
        const GLuint& GetIndexBufferId() const;

        const uint32_t& GetNumIndices() const;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
} // namespace MeowEngine


#endif //PHYSICAT_OPENGL_MESH_HPP
