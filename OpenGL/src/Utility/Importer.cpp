
#include "Importer.h"

#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <map>
#include "vec3.hpp"
#include "gtx/hash.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


Model importObj(std::string const& filename)
{
    struct Vertex
    {
        float x;
        float y;
        float z;

        float j;
        float k;
        float l;

        float u;
        float v;
    };

    struct indx_orderable
    {
        tinyobj::index_t indx;

        bool operator<(const indx_orderable& rhs) const
        {
            if (indx.vertex_index < rhs.indx.vertex_index)
            {
                return true;
            }
            else if (indx.vertex_index == rhs.indx.vertex_index)
            {
                if (indx.normal_index < rhs.indx.normal_index)
                {
                    return true;
                }
                else if (indx.normal_index == rhs.indx.normal_index)
                {
                    return indx.texcoord_index < rhs.indx.texcoord_index;
                }
            }

            return false;
        }

        explicit(false) indx_orderable(tinyobj::index_t i) : indx(i) {}
    };

    //If this is not true, Nasal Demons.
    static_assert(sizeof(Vertex) == (sizeof(float) * 8));

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;
    std::map<indx_orderable, size_t> vertCache;

    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &error, filename.c_str(), (filename + "/../").c_str(), true))
    {
        std::cerr << "An error has occurred while loading file: " << filename << "\n" <<
            error << std::endl;
        assert(false);
    }

    if (!warning.empty()) {
        std::cout << warning << std::endl;
    }

    if (!error.empty()) {
        std::cerr << error << std::endl;
    }

    std::vector<Vertex> verts;   // The number of vertices has a correspondence with the number of specified materials.
    std::vector<unsigned int> indices;

    // We work under the assumption of triangulation
    size_t curr_top = 0; // we store the top-most vertex in a separate variable for convenience.

    for (auto const& shape : shapes)
    {
        for (size_t triangle = 0ui32; triangle < shape.mesh.indices.size() / 3; ++triangle)
        {
            for (size_t vert = 0; vert < 3; ++vert)
            {
                auto const& vert_indxs = shape.mesh.indices[3 * triangle + vert];

                if (vertCache.contains(vert_indxs))
                {
                    indices.emplace_back(vertCache[vert_indxs]);
                    continue;
                }

                //Boilerplate-y
                verts.emplace_back(
                    attributes.vertices[vert_indxs.vertex_index * 3],
                    attributes.vertices[vert_indxs.vertex_index * 3 + 1],
                    attributes.vertices[vert_indxs.vertex_index * 3 + 2],

                    attributes.normals[vert_indxs.normal_index * 3],
                    attributes.normals[vert_indxs.normal_index * 3 + 1],
                    attributes.normals[vert_indxs.normal_index * 3 + 2],

                    attributes.texcoords[vert_indxs.texcoord_index * 2],
                    attributes.texcoords[vert_indxs.texcoord_index * 2 + 1]
                );

                indices.emplace_back(curr_top);
                vertCache[vert_indxs] = curr_top++;
            }
        }
    }

    std::vector<float> raw_verts(verts.size()*8);
    memcpy(raw_verts.data(), verts.data(), verts.size() * sizeof(Vertex)); //pray for the best.

    return Model(std::move(raw_verts), std::move(indices));
}   
