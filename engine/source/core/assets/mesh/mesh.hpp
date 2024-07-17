//
// Created by Akira Lynn on 08/09/22.
//

#ifndef PHYSICAT_MESH_HPP
#define PHYSICAT_MESH_HPP

#include "internal_ptr.hpp"
#include "vertex.hpp"
#include "vector"

namespace physicat {
    struct Mesh {
        Mesh(const std::vector<physicat::Vertex>& vertices, const std::vector<uint32_t>& indices);

        const std::vector<physicat::Vertex>& GetVertices() const;
        const std::vector<uint32_t> & GetIndices() const; // Is it possible to dynamically use int type for different meshes

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat


#endif //PHYSICAT_MESH_HPP
