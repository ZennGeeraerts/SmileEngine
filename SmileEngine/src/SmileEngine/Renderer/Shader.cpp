#include "smpch.h"
#include "Shader.h"

#include "Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#include "Platform/DirectX11/DirectX11Context.h"
#include "Platform/DirectX11/DirectX11Shader.h"
#endif

namespace Smile
{
	Shader* Shader::Create(RenderingContext* pRenderingContext, const std::string& filePath, const BufferLayout& bufferLayout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "Shader::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return new DirectX11Shader(static_cast<DirectX11Context*>(pRenderingContext), filePath, bufferLayout);
#endif
		}

		SM_ASSERT(false, "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}