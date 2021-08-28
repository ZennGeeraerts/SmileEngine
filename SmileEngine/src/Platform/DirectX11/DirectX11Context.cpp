#include "smpch.h"
#include "DirectX11Context.h"

#include "SmileEngine/Window.h"
#include "SmileEngine/Core.h"
#include "SmileEngine/Logger.h"

#include "DirectX11Buffer.h"

namespace Smile
{
	DirectX11Context::DirectX11Context(Window* pWindow)
		: m_pWindow{ pWindow }
	{
	}

	DirectX11Context::~DirectX11Context()
	{
		SAFE_RELEASE(m_pDepthStencilBuffer);
		SAFE_RELEASE(m_pRenderTargetBuffer);
		SAFE_RELEASE(m_pCurrentRenderTarget);
		SAFE_RELEASE(m_pDepthStencilView);
		SAFE_RELEASE(m_pDXGIFactory);
		SAFE_RELEASE(m_pSwapChain);

		if (m_pDeviceContext)
		{
			m_pDeviceContext->ClearState();
			m_pDeviceContext->Flush();
			SAFE_RELEASE(m_pDeviceContext);
		}

		SAFE_RELEASE(m_pDevice);
	}

	void DirectX11Context::Init()
	{
		// Create Device and Device context, using hardware acceleration
		D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
		uint32_t createDeviceFlags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &m_pDevice, &featureLevel, &m_pDeviceContext);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to create D3D11Device");
			return;
		}

		// Create DXGI Factory to create SwapChain based on hardware
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_pDXGIFactory));
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to create DXGIFactory");
			return;
		}

		// TODO: Get width and height from window
		const unsigned int width = m_pWindow->GetWidth() - 17.5f;
		const unsigned int height = m_pWindow->GetHeight() - 37.f;

		// Create SwapChain Descriptor
		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		swapChainDesc.OutputWindow = static_cast<HWND>(m_pWindow->GetNativeWindow());

		// Create SwapChain and hook it into the handle of the SDL window
		result = m_pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to create swap chain");
			return;
		}

		/*------------------------------------- Render Target Code -------------------------------------*/
		// Create the Depth/Stencil Buffer and View
		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to create depth stencil buffer");
			return;
		}

		result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to create depth stencil view");
			return;
		}

		// Create the RenderTargetView
		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to get buffer from swap chain");
			return;
		}

		result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer, 0, &m_pCurrentRenderTarget);
		if (FAILED(result))
		{
			SM_LOG_ERROR("DirectXContext::Init > Failed to create render target view");
			return;
		}

		m_pDeviceContext->OMSetRenderTargets(1, &m_pCurrentRenderTarget, m_pDepthStencilView);
		/*------------------------------------- Render Target Code End -------------------------------------*/

		// Set the Viewport
		D3D11_VIEWPORT viewPort{};
		viewPort.Width = width;
		viewPort.Height = height;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0;
		m_pDeviceContext->RSSetViewports(1, &viewPort);
	}

	void DirectX11Context::ClearBuffer()
	{
		m_pDeviceContext->OMSetRenderTargets(1, &m_pCurrentRenderTarget, m_pDepthStencilView);

		const float clearColor[]{ 0, 0, 0.3f, 1.f };
		m_pDeviceContext->ClearRenderTargetView(m_pCurrentRenderTarget, clearColor);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void DirectX11Context::PresentBackbuffer()
	{
		m_pSwapChain->Present(0, 0);
	}
}