#include "smpch.h"
#include "VertexArray.h"

#include "Renderer.h"

namespace Smile
{
	VertexBuffer* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "VertexArray::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return nullptr;

		case RendererAPI::API::eSmileRaster:
			return nullptr;
#endif
		}

		SM_ASSERT(false, "VertexArray::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}