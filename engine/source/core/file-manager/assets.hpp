//
// Created by Akira Lynn on 11/09/22.
//

#ifndef PHYSICAT_ASSETS_HPP
#define PHYSICAT_ASSETS_HPP

#include "mesh.hpp"
#include <string>
#include "bitmap.hpp"

namespace MeowEngine::assets {
    std::string LoadTextFile(const std::string& path);
    MeowEngine::Mesh LoadObjFile(const std::string& path);
    MeowEngine::Bitmap LoadBitmap(const std::string& path);
} // namespace MeowEngine::assets;

#endif //PHYSICAT_ASSETS_HPP
