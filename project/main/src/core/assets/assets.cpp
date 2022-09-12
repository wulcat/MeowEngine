//
// Created by Akira Lynn on 11/09/22.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include "assets.hpp"
#include "../wrappers/sdl_wrapper.hpp"
#include "mesh/vertex.hpp"
#include "sstream"
#include "tiny_obj_loader.h"
#include "unordered_map"
#include "vector"
#include "../logger/log.hpp"
#include <string>


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

    return physicat::Mesh(vertices, indices);
}