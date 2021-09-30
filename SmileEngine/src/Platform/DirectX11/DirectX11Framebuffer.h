#pragma once
#include "SmileEngine/Renderer/Framebuffer.h"
#include "DirectX11Context.h"

namespace Smile
{
	class DirectX11Framebuffer final : public Framebuffer
	{
	public:
		DirectX11Framebuffer(const FramebufferData& framebufferData);
		virtual ~DirectX11Framebuffer();

		virtual void Invalidate() override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) override;
		virtual void Clear() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FramebufferData& GetData() const override { return m_Data; }
		virtual void* GetColor() const override { return m_pColorShaderResourceView; }

	private:
		DirectX11Context* m_pDirectX11Context = nullptr;
		FramebufferData m_Data;

		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
		ID3D11Texture2D* m_pColorBuffer = nullptr;
		ID3D11ShaderResourceView* m_pColorShaderResourceView = nullptr;

		//ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
		//ID3D11Texture2D* m_pDepthStencilBuffer = nullptr;

		DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };
	};
}
