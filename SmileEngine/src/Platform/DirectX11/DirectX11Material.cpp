#include "smpch.h"
#include "DirectX11Material.h"

namespace Smile
{
	DirectX11Material::DirectX11Material(const Ref<Shader>& pShader)
	{
		m_pShader = std::dynamic_pointer_cast<DirectX11Shader>(pShader);
		SM_ASSERT(m_pShader, "DirectX11Material > Shader is not a DirectX11Shader");
	}

	DirectX11Material::~DirectX11Material()
	{
		m_pTextures2D.clear();
	}

	void DirectX11Material::Bind() const
	{
		m_pShader->Bind();
	}

	void DirectX11Material::SetTexture2D(const std::string sementicName, const Ref<Texture2D>& pTexture)
	{
		
	}
}