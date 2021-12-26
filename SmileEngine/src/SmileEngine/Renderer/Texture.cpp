#include "smpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/DirectX11/DirectX11Texture.h"

namespace Smile
{
	Ref<Texture2D> Texture2D::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "Shader::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return CreateRef<DirectX11Texture2D>(filePath);

		case RendererAPI::API::eSmileRaster:
			return nullptr;
#endif
		}

		SM_ASSERT(false, "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}