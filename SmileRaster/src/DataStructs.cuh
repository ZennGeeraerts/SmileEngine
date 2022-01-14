#pragma once
#include <stdint.h>
#include <DirectXMath.h>
#include "../../SmileRaster/Libs/glm/glm.hpp"

namespace Smile
{
	namespace Raster
	{
		struct VertexShaderInput final
		{
			glm::vec3 Position{};
			glm::vec3 Color{};
		};

		struct VertexShaderOutput final
		{
			glm::vec4 Position{};
			glm::vec3 Color{};
		};

		using InterpolatedAttributes = VertexShaderOutput;

		struct ShaderData final
		{
			glm::mat4 ViewProjection{ 1.0f };
			glm::mat4 World{ 1.0f };
			glm::mat4 ViewInverse{ 1.0f };
		};

		enum class ColorbufferFormat
		{
			eRGB,
			eRGBA
		};

		struct Framebuffer final
		{
			uint8_t* d_Colorbuffer{ nullptr };
			float* d_Depthbuffer{ nullptr };
			InterpolatedAttributes* d_PixelData{ nullptr };

			uint32_t Width = 0;
			uint32_t Height = 0;
			uint8_t ColorChannelCount = 3;
			uint8_t* pOutput{ nullptr };
		};

		struct VertexBuffer final
		{
			void* d_Vertices = nullptr;
			uint32_t ByteWidth = 0;

			VertexShaderOutput* d_VertexShaderOutput = nullptr;
		};

		struct IndexBuffer final
		{
			uint32_t* d_Indices = nullptr;
		};

		struct Triangle final
		{
			union
			{
				VertexShaderOutput Vertices[3]{};
				struct
				{
					VertexShaderOutput Vertex0;
					VertexShaderOutput Vertex1;
					VertexShaderOutput Vertex2;
				};
			};
		};

		struct Bin final
		{
			uint32_t Queue[1024]{};
			uint32_t QueueSize = 0;
		};
	}
}