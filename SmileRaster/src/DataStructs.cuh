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
			DirectX::XMFLOAT4X4 ViewProjection{};
			DirectX::XMFLOAT4X4 World{};
			DirectX::XMFLOAT4X4 ViewInverse{};
		};

		struct VS_INPUT final
		{
			glm::vec3 Position{};
		};

		struct VS_OUTPUT final
		{
			glm::vec3 Position{};
		};

		struct Triangle final
		{
			union
			{
				VS_INPUT Vertices[3]{};
				struct
				{
					VS_INPUT Vertex0;
					VS_INPUT Vertex1;
					VS_INPUT Vertex2;
				};
			};
		};
	}
}