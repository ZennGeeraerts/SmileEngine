#pragma once
#include <stdint.h>
#include <DirectXMath.h>
#include "../../SmileRaster/Libs/glm/glm.hpp"

namespace Smile
{
	namespace Raster
	{
		struct VertexBuffer final
		{
			void* d_Vertices = nullptr;
			uint32_t Stride{ 0 };
			uint32_t Count{ 0 };
		};

		struct IndexBuffer final
		{
			uint32_t* d_Indices = nullptr;
			uint32_t Count = 0;
		};

		struct ShaderData final
		{
			glm::mat4 ViewProjection{ 1.0f };
			glm::mat4 World{ 1.0f };
			glm::mat4 ViewInverse{ 1.0f };
		};

		struct VS_INPUT final
		{
			glm::vec3 Position{};
			glm::vec3 Color{};
		};

		struct VS_OUTPUT final
		{
			glm::vec4 Position{};
			glm::vec3 Color{};
		};

		struct Triangle final
		{
			union
			{
				VS_OUTPUT Vertices[3]{};
				struct
				{
					VS_OUTPUT Vertex0;
					VS_OUTPUT Vertex1;
					VS_OUTPUT Vertex2;
				};
			};
		};
	}
}