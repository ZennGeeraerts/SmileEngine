#include "smpch.h"
#include "Material.h"

#include "Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#include "Platform/DirectX11/DirectX11Material.h"
#endif

namespace Smile
{
	Ref<Material> Material::Create(const Ref<Shader>& pShader)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::eNone:
			SM_ASSERT(false, "Material::Create > return nullptr, no renderer api selected");
			return nullptr;

#ifdef SM_PLATFORM_WINDOWS
		case RendererAPI::API::eDirectX11:
			return std::make_shared<DirectX11Material>(pShader);
#endif
		}

		SM_ASSERT(false, "Material::Create > return nullptr, unknown render api or render api is not supported on this platform");
		return nullptr;
	}
}