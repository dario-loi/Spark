
#include "Importer.h"

#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "vec3.hpp"
#include "gtx/hash.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

Model importObj(std::string const& filename)
{

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;
    std::unordered_map<glm::vec3, unsigned int> uniqueVertices;

    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &error, filename.c_str(), (filename + "/../").c_str(), true))
    {
        std::cerr << "An error has occurred while loading file: " << filename << "\n" <<
            error << std::endl;
        assert(false);
    }

    std::vector<float> raw_verts;
    std::vector<unsigned int> indices;

    for (auto const& shape : shapes)
    {
        for (auto const& indx : shape.mesh.indices)
        {
            
            for(unsigned int i = 0; i < 3; ++i)
            {
                raw_verts.emplace_back(attributes.vertices[3ui64 * indx.vertex_index + i]);
            }
            for (unsigned int i = 0; i < 3; ++i)
            {
                raw_verts.emplace_back(attributes.normals[3ui64 * indx.normal_index + i]);
            }
            for (unsigned int i = 0; i < 2; ++i)
            {
                raw_verts.emplace_back(attributes.texcoords[2ui64 * indx.texcoord_index + i]);
            }

            glm::vec3 position = {
                attributes.vertices[3ui64 * indx.vertex_index],
                attributes.vertices[3ui64 * indx.vertex_index + 1ui64],
                attributes.vertices[3ui64 * indx.vertex_index + 2ui64]
            };

            if (!uniqueVertices.contains(position)) {
                uniqueVertices[position] = static_cast<uint32_t>((raw_verts.size()/8) - 1);
                indices.emplace_back(uniqueVertices[position]);
            }
            else
            {

                std::vector<float> v1(std::next(raw_verts.end(), -8), raw_verts.end());
                std::vector<float> v2(std::next(raw_verts.begin(), uniqueVertices[position] * 8), std::next(raw_verts.begin(), 8 + uniqueVertices[position] * 8));

                if (v1 == v2)
                {
                    indices.emplace_back(uniqueVertices[position]);
                }
                else
                {
                    indices.emplace_back(static_cast<unsigned int>((raw_verts.size() / 8) - 1));
                }

            }
        }
    }

    return Model(std::move(raw_verts), std::move(indices));
}   
