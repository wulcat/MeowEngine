//
// Created by Akira M on 11/09/22.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include "assets.hpp"
#include "sdl_wrapper.hpp"
#include "mesh/vertex.hpp"
//#include "sstream"
#include "tiny_obj_loader.h"
#include "unordered_map"
#include "vector"
//#include "../logger/log.hpp"
#include <string>
#include <SDL_image.h>


std::string physicat::assets::LoadTextFile(const std::string &path) {
    // Creates an abstract structure for I/O, we only care
    // for result and so treat it as a pointer
    // 'r' stands for Read mode
    SDL_RWops* file{
        SDL_RWFromFile(path.c_str(), "r")
    };

    auto fileLength(static_cast<size_t>(SDL_RWsize(file)));

    // We now actually load the file
    // '1' means we close the file
    // void* pointer is returned (raw data in memory)
    // , so we need to cast it into appropriate to consume it
    void* data{
        SDL_LoadFile_RW(file, nullptr, 1)
    };

    std::string result(static_cast<char*>(data), fileLength);

    // Once cast we clear the data void* to avoid memory leak
    // (pretty interesting as we are cleaning up the pointer)
    SDL_free(data);

    return result;
}

physicat::Mesh physicat::assets::LoadObjFile(const std::string &path) {
    std::istringstream sourceStream(physicat::assets::LoadTextFile(path));

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;

    if(!tinyobj::LoadObj(
            &attributes,
            &shapes,
            &materials,
            &error,
            &sourceStream
    )) {
        throw std::runtime_error("physicat::assets::LoadObjFile: Error: "+error);
    }

    std::vector<physicat::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<glm::vec3, uint32_t> uniqueVertices;

    for(const auto& shape : shapes) {
        for(const auto& index : shape.mesh.indices) {
            glm::vec3 position {
                attributes.vertices[3 * index.vertex_index + 0],
                attributes.vertices[3 * index.vertex_index + 1],
                attributes.vertices[3 * index.vertex_index + 2]
            };

            if(uniqueVertices.count(position) == 0) {
                uniqueVertices[position] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(physicat::Vertex{position});
            }

            indices.push_back(uniqueVertices[position]);
        }
    }

    return { vertices, indices };
}

physicat::Bitmap physicat::assets::LoadBitmap(const std::string &path) {
    SDL_RWops* file {SDL_RWFromFile(path.c_str(), "rb")}; // rb denotes to load binary data
    SDL_Surface* source {IMG_Load_RW(file, 1)};
    SDL_Rect imageFrame {0, 0, source->w, source->h};

    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t alphaMask;

    // what is Endian?
#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    redMask = 0xff000000;
    greenMask = 0x00ff0000;
    blueMask = 0x0000ff00;
    alphaMask = 0x000000ff;
#else
    redMask = 0x000000ff;
    greenMask = 0x0000ff00;
    blueMask = 0x00ff0000;
    alphaMask = 0xff000000;
#endif

    SDL_Surface* target {
        SDL_CreateRGBSurface(
            0,
            imageFrame.w,
            imageFrame.h,
            32,
            redMask, greenMask, blueMask, alphaMask
        )
    };

    // copy data from source to target
    SDL_BlitSurface(source, &imageFrame, target, &imageFrame);
    SDL_FreeSurface(source);

    return physicat::Bitmap(target);
}