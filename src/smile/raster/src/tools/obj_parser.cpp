#include "obj_parser.h"

#include <gtx/projection.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace smile::Raster::objparser
{
    Mesh LoadObj(const std::filesystem::path& path)
    {
        Mesh mesh{};

        std::ifstream inputFile{ path, std::ios::in };
        if (!inputFile)
        {
            std::cerr << "Cannot open " << path << '\n';
            return mesh;
        }

        std::string line;
        while (std::getline(inputFile, line))
        {
            const std::string prefix = line.substr(0, 2);

            if (prefix == "v ")
            {
                std::istringstream positionStream{ line.substr(2) };

                glm::vec3 position{};
                positionStream >> position.x;
                positionStream >> position.y;
                positionStream >> position.z;

                mesh.Positions.emplace_back(position);
            }
            else if (prefix == "vt")
            {
                std::istringstream texCoordsStream{ line.substr(2) };

                glm::vec2 texCoords{};
                texCoordsStream >> texCoords.x;
                texCoordsStream >> texCoords.y;

                mesh.TextureCoords.emplace_back(texCoords);
            }
            else if (prefix == "vn")
            {
                std::istringstream normalStream{ line.substr(2) };

                glm::vec3 normal{};
                normalStream >> normal.x;
                normalStream >> normal.y;
                normalStream >> normal.z;

                mesh.Normals.emplace_back(normal);
            }
            else if (prefix == "f ")
            {
                std::istringstream values{ line.substr(2) };

                uint32_t vi0;
                uint32_t vti0;
                uint32_t vni0;

                uint32_t vi1;
                uint32_t vti1;
                uint32_t vni1;

                uint32_t vi2;
                uint32_t vti2;
                uint32_t vni2;
               
                sscanf(line.c_str(), "f %i/%i/%i %i/%i/%i %i/%i/%i", &vi0, &vti0, &vni0, &vi1, &vti1, &vni1, &vi2, &vti2, &vni2);

                --vi0;
                --vni0;
                --vti0;

                --vi1;
                --vni1;
                --vti1;

                --vi2;
                --vni2;
                --vti2;

                Vertex v0{ mesh.Positions[vi0], mesh.TextureCoords[vti0], mesh.Normals[vni0] };
                Vertex v1{ mesh.Positions[vi1], mesh.TextureCoords[vti1], mesh.Normals[vni1] };
                Vertex v2{ mesh.Positions[vi2], mesh.TextureCoords[vti2], mesh.Normals[vni2] };

                mesh.VertexBuffer.emplace_back(v0);
                mesh.IndexBuffer.emplace_back(mesh.VertexBuffer.size() - 1);

                mesh.VertexBuffer.emplace_back(v1);
                mesh.IndexBuffer.emplace_back(mesh.VertexBuffer.size() - 1);

                mesh.VertexBuffer.emplace_back(v2);
                mesh.IndexBuffer.emplace_back(mesh.VertexBuffer.size() - 1);
            }
        }

        for (size_t i{}; i < mesh.IndexBuffer.size(); i += 3)
        {
            uint32_t index0 = mesh.IndexBuffer[i];
            uint32_t index1 = mesh.IndexBuffer[i + 1];
            uint32_t index2 = mesh.IndexBuffer[i + 2];

            const glm::vec3& p0 = mesh.VertexBuffer[index0].Position;
            const glm::vec3& p1 = mesh.VertexBuffer[index1].Position;
            const glm::vec3& p2 = mesh.VertexBuffer[index2].Position;

            const glm::vec2& uv0 = mesh.VertexBuffer[index0].TextureCoords;
            const glm::vec2& uv1 = mesh.VertexBuffer[index1].TextureCoords;
            const glm::vec2& uv2 = mesh.VertexBuffer[index2].TextureCoords;

            const glm::vec3 edge0 = p1 - p0;
            const glm::vec3 edge1 = p2 - p0;

            const glm::vec3 diffX{ uv1.x - uv0.x, uv2.x - uv0.x, 0.0f };
            const glm::vec3 diffY{ uv1.y - uv0.y, uv2.y - uv0.y, 0.0f };

            float r = 1.0f / glm::cross(diffX, diffY).length();

            glm::vec3 tangent = (edge0 * diffY.y - edge1 * diffY.x) * r;

            mesh.VertexBuffer[index0].Tangent += tangent;
            mesh.VertexBuffer[index1].Tangent += tangent;
            mesh.VertexBuffer[index2].Tangent += tangent;
        }

        for (auto& vertex : mesh.VertexBuffer)
            vertex.Tangent = glm::normalize(vertex.Tangent - glm::proj(vertex.Tangent, vertex.Normal));

        return mesh;
    }
}