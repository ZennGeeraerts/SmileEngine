#pragma once
#include <stdint.h>
#include <DirectXMath.h>

namespace Smile
{
	namespace Raster
	{
		struct VertexBuffer final
		{
			void* d_Vertices = nullptr;
			uint32_t Stride = 0;
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
			DirectX::XMFLOAT3 Position;
		};

		struct VS_OUTPUT final
		{
			DirectX::XMFLOAT3 Position;
		};

		struct Triangle final
		{
			VS_INPUT Vertices[3]{};
		};
	}
}