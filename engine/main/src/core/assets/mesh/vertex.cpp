//
// Created by Akira Lynn on 08/09/22.
//

#include "vertex.hpp"

using physicat::Vertex;

bool Vertex::operator==(const physicat::Vertex &other) const {
    return Position == other.Position && TextureCoord == other.TextureCoord;
}