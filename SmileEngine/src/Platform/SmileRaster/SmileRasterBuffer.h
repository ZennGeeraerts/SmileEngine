#pragma once
#include "SmileEngine/Renderer/Buffer.h"
#include "SmileRasterContext.h"

namespace Smile
{
	class SmileRasterVertexBuffer final : public VertexBuffer
	{
	public:
		SmileRasterVertexBuffer(const VertexBufferData& vertexBufferData);
		virtual ~SmileRasterVertexBuffer() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		SmileRasterContext* m_pSmileRasterContext = nullptr;
		BufferLayout m_Layout;
		void* m_pVertices = nullptr;
		uint32_t m_Count;
	};

	class SmileRasterIndexBuffer final : public IndexBuffer
	{
	public:
		SmileRasterIndexBuffer(const IndexBufferData& indexBufferData);
		virtual ~SmileRasterIndexBuffer() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		SmileRasterContext* m_pSmileRasterContext = nullptr;
		uint32_t* m_pIndices = nullptr;
		uint32_t m_Count;
	};
}

