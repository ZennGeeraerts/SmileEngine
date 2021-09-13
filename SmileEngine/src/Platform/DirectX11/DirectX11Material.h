#pragma once

#include "SmileEngine/Renderer/Material.h"
#include "DirectX11Shader.h"
#include "DirectX11Texture.h"

namespace Smile
{
	class DirectX11Material final : public Material
	{
	public:
		DirectX11Material(const Ref<Shader>& pShader);
		~DirectX11Material();
		
		virtual void Bind() const override;
		virtual void SetTexture2D(const std::string sementicName, const Ref<Texture2D>& pTexture) override;

	private:
		std::vector<Ref<DirectX11Texture2D>> m_pTextures2D;
		Ref<DirectX11Shader> m_pShader;
	};
}
