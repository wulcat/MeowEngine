//
// Created by Akira Lynn on 08/09/22.
//

#include "mesh.hpp"

#include <utility>

using physicat::Mesh;

struct Mesh::Internal {
    const std::vector<physicat::Vertex> Vertices;
    const std::vector<uint32_t> Indices;

    Internal(std::vector<physicat::Vertex> vertices, std::vector<uint32_t> indices) :
        Vertices(std::move(vertices)),
        Indices(std::move(indices))
    {}
};

physicat::Mesh::Mesh(const std::vector<physicat::Vertex> &vertices, const std::vector<uint32_t> &indices)
    : InternalPointer(physicat::make_internal_ptr<Internal>(vertices, indices)) {
}

const std::vector<physicat::Vertex> &physicat::Mesh::GetVertices() {
    return InternalPointer->Vertices;
}

const std::vector<uint32_t> &physicat::Mesh::GetIndices() {
    return InternalPointer->Indices;
}

