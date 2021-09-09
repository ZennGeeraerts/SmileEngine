#pragma once
#include "SmileEngine/Renderer/Buffer.h"
#include "DirectX11Context.h"

namespace Smile
{
	class DirectX11VertexBuffer final : public VertexBuffer
	{
	public:
		DirectX11VertexBuffer(const VertexBufferData& vertexBufferData);
		virtual ~DirectX11VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		DirectX11Context* m_pDirectX11Context;
		ID3D11Buffer* m_pVertexBuffer;
		BufferLayout m_Layout;
	};

	class DirectX11IndexBuffer final : public IndexBuffer
	{
	public:
		DirectX11IndexBuffer(const IndexBufferData& indexBufferData);
		virtual ~DirectX11IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		DirectX11Context* m_pDirectX11Context;
		ID3D11Buffer* m_pIndexBuffer;
		uint32_t m_Count;
	};

	D3D11_USAGE BufferUsageToDirectXType(BufferUsage bufferUsage);
}

