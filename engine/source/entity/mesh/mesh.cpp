//
// Created by Akira Lynn on 08/09/22.
//

#include "mesh.hpp"

#include <utility>

using MeowEngine::Mesh;

struct Mesh::Internal {
    const std::vector<MeowEngine::Vertex> Vertices;
    const std::vector<uint32_t> Indices;

    Internal(std::vector<MeowEngine::Vertex> vertices, std::vector<uint32_t> indices) :
        Vertices(std::move(vertices)),
        Indices(std::move(indices))
    {}
};

MeowEngine::Mesh::Mesh(const std::vector<MeowEngine::Vertex> &vertices, const std::vector<uint32_t> &indices)
    : InternalPointer(MeowEngine::make_internal_ptr<Internal>(vertices, indices)) {
}

const std::vector<MeowEngine::Vertex> &MeowEngine::Mesh::GetVertices() const {
    return InternalPointer->Vertices;
}

const std::vector<uint32_t> & MeowEngine::Mesh::GetIndices() const {
    return InternalPointer->Indices;
}

