#pragma once

namespace Smile
{
	class RenderingContext;

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(RenderingContext* pRenderingContext, float* pVertices, uint32_t count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(RenderingContext* pRenderingContext, uint32_t* pIndices, uint32_t count);
	};
}