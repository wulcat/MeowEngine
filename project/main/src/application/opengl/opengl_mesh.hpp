//
// Created by Akira Lynn on 12/09/22.
//

#ifndef PHYSICAT_OPENGL_MESH_HPP
#define PHYSICAT_OPENGL_MESH_HPP

#include "../../core/wrappers/graphics_wrapper.hpp"
#include "../../core/assets/mesh/mesh.hpp"
#include "../../core/internal_ptr.hpp"

namespace physicat {
    struct OpenGLMesh {
        explicit OpenGLMesh(const physicat::Mesh& mesh);

        const GLuint& GetVertexBufferId() const;
        const GLuint& GetIndexBufferId() const;

        const uint32_t& GetNumIndices() const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat


#endif //PHYSICAT_OPENGL_MESH_HPP
