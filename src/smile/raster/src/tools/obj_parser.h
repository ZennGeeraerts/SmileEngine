#pragma once
#include <glm.hpp>
#include <vector>
#include <filesystem>

namespace smile::Raster::objparser
{
	struct Vertex final
	{
		Vertex() = default;
		Vertex(const glm::vec3& pos, const glm::vec2& texCoords, const glm::vec3& normal)
			: Position{ pos }, TextureCoords{ texCoords }, Normal{ normal }
		{
		}

		glm::vec3 Position{};
		glm::vec2 TextureCoords{};
		glm::vec3 Normal{};
		glm::vec3 Tangent{};
	};

	struct Mesh final
	{
		std::vector<glm::vec3> Positions{};
		std::vector<glm::vec2> TextureCoords{};
		std::vector<glm::vec3> Normals{};

		std::vector<Vertex> VertexBuffer{};
		std::vector<uint32_t> IndexBuffer{};
	};

	Mesh LoadObj(const std::filesystem::path& path);
}