#include "smpch.h"
#include "Framebuffer.h"

#include "Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#include "Platform/DirectX11/DirectX11Framebuffer.h"
#endif

namespace Smile
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferData& framebufferData)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "Framebuffer::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return CreateRef<DirectX11Framebuffer>(framebufferData);

		case RendererAPI::API::eSmileRaster:
			return nullptr;
#endif
		}

		SM_ASSERT(false, "Framebuffer::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}