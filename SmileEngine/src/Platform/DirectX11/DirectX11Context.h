#pragma once

#include "SmileEngine/Renderer/RenderingContext.h"

#include <d3d11.h>

namespace Smile
{
	class RenderTarget;

	class DirectX11Context final : public RenderingContext
	{
	public:
		DirectX11Context(HWND* pWindowHandle);
		~DirectX11Context();

		void Init() override;
		void SwapBuffers() override;

		inline ID3D11Device* GetDevice() const { return m_pDevice; }
		inline ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }

	private:
		void SetRenderTarget(RenderTarget* pRenderTarget);

	private:
		HWND* m_pWindowHandle = nullptr;

		ID3D11Device* m_pDevice = nullptr;
		ID3D11DeviceContext* m_pDeviceContext = nullptr;
		IDXGIFactory* m_pDXGIFactory = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;

		RenderTarget* m_pDefaultRenderTarget = nullptr;
		RenderTarget* m_pCurrentRenderTarget = nullptr;
	};
}
