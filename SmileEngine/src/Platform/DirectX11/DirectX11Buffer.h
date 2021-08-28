#pragma once
#include "SmileEngine/Renderer/Buffer.h"

namespace Smile
{
	class DirectX11Context;

	class DirectX11VertexBuffer final : public VertexBuffer
	{
	public:
		DirectX11VertexBuffer(DirectX11Context* pDirectX11Context, void* pVertices, uint32_t stride, uint32_t count);
		virtual ~DirectX11VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		DirectX11Context* m_pDirectX11Context;
		ID3D11Buffer* m_pVertexBuffer;
		uint32_t m_Stride;
	};

	class DirectX11IndexBuffer final : public IndexBuffer
	{
	public:
		DirectX11IndexBuffer(DirectX11Context* pDirectX11Context, uint32_t* pIndices, uint32_t count);
		virtual ~DirectX11IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		DirectX11Context* m_pDirectX11Context;
		ID3D11Buffer* m_pIndexBuffer;
		uint32_t m_AmountIndices;
	};
}

