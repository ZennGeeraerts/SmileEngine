#pragma once

#include "SmileEngine/Renderer/Texture.h"
#include "DirectX11Context.h"

namespace Smile
{
	class DirectX11Texture2D final : public Texture2D
	{
	public:
		DirectX11Texture2D(const std::string& filePath);
		virtual ~DirectX11Texture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pShaderResourceView; }

	private:
		bool LoadTexture(const std::string& filePath);

	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		DirectX11Context* m_pDirectX11Context = nullptr;
		ID3D11Resource* m_pTexture = nullptr;
		ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;
	};
}

