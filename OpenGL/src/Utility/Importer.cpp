
#include "Importer.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <map>
#include "vec3.hpp"
#include "gtx/hash.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"


sparkutils::SparkImportReturn sparkutils::importObj(std::string const& filename)
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

        float tj;
        float tk;
        float tl;

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

    //If this is not true, Nasal Demons, should be true since Vertex is POD.
    static_assert(sizeof(Vertex) == (sizeof(float) * 11));

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
        for (size_t triangle = 0ui64; triangle < shape.mesh.indices.size() / 3; ++triangle)
        {
            /*
                Tangents computation
            */

            std::tuple vertices_indx = { 
                shape.mesh.indices[3 * triangle],
                shape.mesh.indices[3 * triangle + 1],
                shape.mesh.indices[3 * triangle + 2]
            };

            auto const& [first_i, second_i, third_i] = vertices_indx;

            std::tuple positions = {
                glm::vec3(
                attributes.vertices[first_i.vertex_index * 3],
                attributes.vertices[first_i.vertex_index * 3 + 1],
                attributes.vertices[first_i.vertex_index * 3 + 2]
                ),
                glm::vec3(
                attributes.vertices[second_i.vertex_index * 3],
                attributes.vertices[second_i.vertex_index * 3 + 1],
                attributes.vertices[second_i.vertex_index * 3 + 2]
                ),
                glm::vec3(
                attributes.vertices[third_i.vertex_index * 3],
                attributes.vertices[third_i.vertex_index * 3 + 1],
                attributes.vertices[third_i.vertex_index * 3 + 2]
                )
            };

            std::tuple tex_coords = {
                glm::vec2(
                attributes.texcoords[first_i.texcoord_index * 2],
                attributes.texcoords[first_i.texcoord_index * 2 + 1]
                ),
                glm::vec2(
                attributes.texcoords[second_i.texcoord_index * 2],
                attributes.texcoords[second_i.texcoord_index * 2 + 1]
                ),
                glm::vec2(
                attributes.texcoords[third_i.texcoord_index * 2],
                attributes.texcoords[third_i.texcoord_index * 2 + 1]
                )
            };

            auto const& [pos1, pos2, pos3] = positions;
            auto const& [tex1, tex2, tex3] = tex_coords;

            auto pos_delta1 = pos2 - pos1;
            auto pos_delta2 = pos3 - pos1;

            auto tex_delta1 = tex2 - tex1;
            auto tex_delta2 = tex3 - tex1;

            float denom = 1.0f / (tex_delta1.x * tex_delta2.y - tex_delta1.y * tex_delta2.x);

            denom = std::_Is_inf(denom) || std::_Is_nan(denom) ? 1.0f : denom;

            glm::vec3 tangent = (pos_delta1 * tex_delta2.y - pos_delta2 * tex_delta1.y) * denom;

            for (size_t vert = 0; vert < 3; ++vert)
            {
                auto const& vert_indxs = shape.mesh.indices[3 * triangle + vert];

                if (vertCache.contains(vert_indxs))
                {

                    auto indx = vertCache[vert_indxs];
                    indices.emplace_back(indx);

                    verts[indx].tj += tangent.x;
                    verts[indx].tk += tangent.y;
                    verts[indx].tl += tangent.z;
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
                    attributes.texcoords[vert_indxs.texcoord_index * 2 + 1],

                    tangent.x,
                    tangent.y,
                    tangent.z

                );

                indices.emplace_back(curr_top);
                vertCache[vert_indxs] = curr_top++;
            }
        }
    }

    std::vector<float> raw_verts(verts.size() * sizeof(Vertex));
    memcpy(raw_verts.data(), verts.data(), verts.size() * sizeof(Vertex)); //pray for the best.

    return { std::move(raw_verts), std::move(indices) };
}   
