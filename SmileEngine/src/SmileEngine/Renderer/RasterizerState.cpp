#include "smpch.h"
#include "RasterizerState.h"

#include "Renderer.h"

#include "Platform/DirectX11/DirectX11RasterizerState.h"

namespace Smile
{
	Ref<RasterizerState> RasterizerState::Create(const RasterizerStateData& rasterizerStateData)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "RasterizerState::Create > return nullptr, no renderer api selected");
			return nullptr;

		case RendererAPI::API::eDirectX11:
			return CreateRef<DirectX11RasterizerState>(rasterizerStateData);

		case RendererAPI::API::eSmileRaster:
			return nullptr;
		}

		SM_ASSERT(false, "RasterizerState::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}