#pragma once
#include "SmileEngine/Renderer/Buffer.h"
#include "SmileRasterContext.h"

namespace Smile
{
	class SmileRasterVertexBuffer final : public VertexBuffer
	{
	public:
		SmileRasterVertexBuffer(const VertexBufferData& vertexBufferData);
		virtual ~SmileRasterVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		SmileRasterContext* m_pSmileRasterContext;
		ID3D11Buffer* m_pVertexBuffer;
		BufferLayout m_Layout;
	};
}

