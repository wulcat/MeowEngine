//
// Created by Akira Lynn on 08/09/22.
//

#include "vertex.hpp"

using MeowEngine::Vertex;

bool Vertex::operator==(const MeowEngine::Vertex &other) const {
    return Position == other.Position && TextureCoord == other.TextureCoord;
}