#pragma once

#include "Buffer.h"

namespace Smile
{
	class SMILE_API VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& pVertexBuffer) = 0;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& pVertexBuffer) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create();
	};
}