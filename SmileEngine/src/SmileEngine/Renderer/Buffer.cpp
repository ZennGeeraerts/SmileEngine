#include "smpch.h"
#include "Buffer.h"

#include "Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
	#include "Platform/DirectX11/DirectX11Buffer.h"
#endif

namespace Smile
{
	VertexBuffer* VertexBuffer::Create(RenderingContext* pRenderingContext, void* pVertices, uint32_t count, const BufferLayout& layout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "VertexBuffer::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return new DirectX11VertexBuffer(static_cast<DirectX11Context*>(pRenderingContext), pVertices, count, layout);
#endif
		}

		SM_ASSERT(false, "VertexBuffer::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(RenderingContext* pRenderingContext, uint32_t* pIndices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "IndexBuffer::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return new DirectX11IndexBuffer(static_cast<DirectX11Context*>(pRenderingContext), pIndices, count);
#endif
		}

		SM_ASSERT(false, "IndexBuffer::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}