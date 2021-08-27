#pragma once
#include "SmileEngine/Renderer/RenderingContext.h"

namespace Smile
{
	class Window;

	class DirectX11Context final : public RenderingContext
	{
	public:
		DirectX11Context(Window* pWindow);
		virtual ~DirectX11Context();

		DirectX11Context(const DirectX11Context&) = delete;
		DirectX11Context(DirectX11Context&&) = delete;
		DirectX11Context& operator=(const DirectX11Context&) = delete;
		DirectX11Context& operator=(DirectX11Context&&) = delete;

		virtual void Init() override;
		virtual void ClearBuffer() override;
		virtual void PresentBackbuffer() override;

		inline ID3D11Device* GetDevice() const { return m_pDevice; }
		inline ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }

	private:
		Window* m_pWindow = nullptr;
		ID3D11Device* m_pDevice = nullptr;
		ID3D11DeviceContext* m_pDeviceContext = nullptr;
		IDXGIFactory* m_pDXGIFactory = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;

		ID3D11RenderTargetView* m_pCurrentRenderTarget = nullptr;
		ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

		ID3D11Texture2D* m_pDepthStencilBuffer = nullptr;
		ID3D11Resource* m_pRenderTargetBuffer = nullptr;
	};
}
