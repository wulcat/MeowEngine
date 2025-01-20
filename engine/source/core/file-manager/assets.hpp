//
// Created by Akira Mujawar on 11/09/22.
//

#ifndef MEOWENGINE_ASSETS_HPP
#define MEOWENGINE_ASSETS_HPP

#include "mesh.hpp"
#include <string>
#include "bitmap.hpp"

namespace MeowEngine::assets {
    std::string LoadTextFile(const std::string& path);
    MeowEngine::Mesh LoadObjFile(const std::string& path);
    MeowEngine::Bitmap LoadBitmap(const std::string& path);
} // namespace MeowEngine::assets;

#endif //MEOWENGINE_ASSETS_HPP
