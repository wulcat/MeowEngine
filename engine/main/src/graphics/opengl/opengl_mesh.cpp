//
// Created by Akira Lynn on 12/09/22.
//

#include "opengl_mesh.hpp"
#include "../../core/wrappers/glm_wrapper.hpp"

using physicat::OpenGLMesh;

namespace {
    // Refer: https://registry.khronos.org/OpenGL-Refpages/es1.1/xhtml/glBindBuffer.xml for buffer types
    GLuint CreateVertexArrayID() {
        GLuint id;
        glGenVertexArrays(1, &id);

        return id;
    }

    GLuint CreateVertexBuffer(const physicat::Mesh& mesh) {
        std::vector<float> bufferData;

        for(const auto& vertex: mesh.GetVertices()) {
            bufferData.push_back(vertex.Position.x);
            bufferData.push_back(vertex.Position.y);
            bufferData.push_back(vertex.Position.z);

            bufferData.push_back(vertex.TextureCoord.x);
            bufferData.push_back(vertex.TextureCoord.y);
        }

        // Total Buffer data = pos + texturecoord (uv) = 3 + 2 = 5
        // 5 * float size = 20 bytes
        GLuint bufferId;
        glGenBuffers(1, &bufferId); // create empty buffer
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(
            GL_ARRAY_BUFFER,
            bufferData.size() * sizeof(float),
            bufferData.data(),
            GL_STATIC_DRAW
         );

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
    const GLuint VertexArrayID;

    explicit Internal(const physicat::Mesh& mesh)
        : BufferIdVertices(CreateVertexBuffer(mesh))
        , BufferIdIndices(CreateIndexBuffer(mesh))
        , IndicesCount(static_cast<uint32_t>(mesh.GetIndices().size()))
        , VertexArrayID(CreateVertexArrayID())
   {}

    ~Internal() {
        glDeleteVertexArrays(1, &VertexArrayID);
        glDeleteBuffers(1, &BufferIdVertices);
        glDeleteBuffers(1, &BufferIdIndices);
    }
};

physicat::OpenGLMesh::OpenGLMesh(const physicat::Mesh &mesh)
: InternalPointer(physicat::make_internal_ptr<Internal>(mesh)) {}

const GLuint &physicat::OpenGLMesh::GetVertexArrayId() const {
    return InternalPointer->VertexArrayID;
}

const GLuint &physicat::OpenGLMesh::GetVertexBufferId() const {
    return InternalPointer->BufferIdVertices;
}

const GLuint &physicat::OpenGLMesh::GetIndexBufferId() const {
    return InternalPointer->BufferIdIndices;
}

const uint32_t &physicat::OpenGLMesh::GetNumIndices() const {
    return InternalPointer->IndicesCount;
}
