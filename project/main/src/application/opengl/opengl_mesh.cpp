//
// Created by Akira Lynn on 12/09/22.
//

#include "opengl_mesh.hpp"
#include "../../core/wrappers/glm_wrapper.hpp"

using physicat::OpenGLMesh;

namespace {
    // Refer: https://registry.khronos.org/OpenGL-Refpages/es1.1/xhtml/glBindBuffer.xml for buffer types

    GLuint CreateVertexBuffer(const physicat::Mesh& mesh) {
        std::vector<glm::vec3> positions;

        for(const auto& vertex: mesh.GetVertices()) {
            positions.push_back(vertex.Position);
        }

        GLuint bufferId;
        glGenBuffers(1, &bufferId); // create empty buffer
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

        return bufferId;
    }

    GLuint CreateIndexBuffer(const physicat::Mesh& mesh) {
        GLuint bufferId;

        glGenBuffers(1, &bufferId); // create empty buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(uint32_t), mesh.GetIndices().data(), GL_STATIC_DRAW);

        return bufferId;
    }
} // namespace

struct OpenGLMesh::Internal {
    const GLuint BufferIdVertices;
    const GLuint BufferIdIndices;
    const uint32_t IndicesCount;

    explicit Internal(const physicat::Mesh& mesh)
        :BufferIdVertices(CreateVertexBuffer(mesh)),
         BufferIdIndices(CreateIndexBuffer(mesh)),
         IndicesCount(static_cast<uint32_t>(mesh.GetIndices().size()))
   {}

    ~Internal() {
        glDeleteBuffers(1, &BufferIdVertices);
        glDeleteBuffers(1, &BufferIdIndices);
    }
};

physicat::OpenGLMesh::OpenGLMesh(const physicat::Mesh &mesh)
: InternalPointer(physicat::make_internal_ptr<Internal>(mesh)) {}

const GLuint &physicat::OpenGLMesh::GetVertexBufferId() const {
    return InternalPointer->BufferIdVertices;
}

const GLuint &physicat::OpenGLMesh::GetIndexBufferId() const {
    return InternalPointer->BufferIdIndices;
}

const uint32_t &physicat::OpenGLMesh::GetNumIndices() const {
    return InternalPointer->IndicesCount;
}
