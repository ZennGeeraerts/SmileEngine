#pragma once
#include <stdint.h>
#include <DirectXMath.h>
#include <../../SmileRaster/Libs/glm/glm.hpp>
#include <unordered_map>

namespace Smile
{
	namespace Raster
	{
		/*struct InputDescriptor final
		{
			uint32_t Size{};
			char Name[256]{};
		};

		struct InputLayout final
		{
			InputDescriptor Descriptors[16]{};
			uint32_t DescriptorCount{ 0 };
		};*/

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

		struct Shader final
		{
			std::unordered_map<std::string, glm::mat4> Mat4Data{
			{ "ViewProjection", glm::mat4{ 1.0f } },
			{ "World", glm::mat4{ 1.0f } },
			{ "ViewInverse", glm::mat4{ 1.0f } } };
		};

		enum class ColorbufferFormat
		{
			eRGB,
			eRGBA
		};

		struct Framebuffer final
		{
			uint8_t* d_ColorBuffer{ nullptr };
			float* d_DepthBuffer{ nullptr };
			InterpolatedAttributes* d_PixelData{ nullptr };

			uint32_t Width = 0;
			uint32_t Height = 0;
			uint8_t ColorChannelCount = 3;
			uint8_t* pHostOutput{ nullptr };
		};

		struct VertexBuffer final
		{
			void* d_Vertices = nullptr;
			VertexShaderInput* d_VertexShaderInput = nullptr;
			VertexShaderOutput* d_VertexShaderOutput = nullptr;
			uint32_t ByteWidth = 0;
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