//
// Created by Akira Mujawar on 08/09/22.
//

#ifndef MEOWENGINE_MESH_HPP
#define MEOWENGINE_MESH_HPP

#include "internal_ptr.hpp"
#include "vertex.hpp"
#include "vector"

namespace MeowEngine {
    struct Mesh {
        Mesh(const std::vector<MeowEngine::Vertex>& vertices, const std::vector<uint32_t>& indices);

        const std::vector<MeowEngine::Vertex>& GetVertices() const;
        const std::vector<uint32_t> & GetIndices() const; // Is it possible to dynamically use int type for different meshes

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
} // namespace MeowEngine


#endif //MEOWENGINE_MESH_HPP
