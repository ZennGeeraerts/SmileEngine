#include "smpch.h"
#include "RenderingContext.h"

#include "Renderer.h"
#include "SmileEngine/Core/Window.h"

#include "Platform/DirectX11/DirectX11Context.h"
#include "Platform/SmileRaster/SmileRasterContext.h"

namespace Smile
{
	RenderingContext* RenderingContext::Create(Window* pWindow)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "RenderingContext::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return new DirectX11Context{ pWindow };
#endif

		case RendererAPI::API::eSmileRaster:
			return new SmileRasterContext{ pWindow };
		}

		SM_ASSERT(false, "RenderingContext::Create > return nullptr, unknown renderer api or renderer api is not supported on this platform");
		return nullptr;
	}
}