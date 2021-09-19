#include "smpch.h"
#include "DirectX11RendererAPI.h"

#include "DirectX11Shader.h"
#include "SmileEngine/SmileGame.h"

namespace Smile
{
	void DirectX11RendererAPI::Initialize()
	{
		m_pDirectX11Context = static_cast<DirectX11Context*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pDirectX11Context, "DirectX11RendererAPI > RenderingContext is not a DirectX11Context");
	}

	void DirectX11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_pDirectX11Context->OnWindowResize(width, height);

		D3D11_VIEWPORT viewPort{};
		viewPort.TopLeftX = x;
		viewPort.TopLeftY = y;
		viewPort.Width = width;
		viewPort.Height = height;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;

		m_pDirectX11Context->GetDeviceContext()->RSSetViewports(1, &viewPort);
	}

	void DirectX11RendererAPI::SetClearColor(const DirectX::XMFLOAT4& color)
	{
		m_ClearColor = color;
	}

	void DirectX11RendererAPI::Clear()
	{
		auto pDeviceContext = m_pDirectX11Context->GetDeviceContext();
		auto pRenderTargetView = m_pDirectX11Context->GetRenderTargetView();
		auto pDepthStencilView = m_pDirectX11Context->GetDepthStencilView();

		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		const float* pClearColor = reinterpret_cast<const float*>(&m_ClearColor);
		pDeviceContext->ClearRenderTargetView(pRenderTargetView, pClearColor);
		pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void DirectX11RendererAPI::DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader)
	{
		auto pDirectX11Shader = static_cast<DirectX11Shader*>(pShader.get());
		SM_ASSERT(pDirectX11Shader, "DirectX11RendererAPI::DrawIndexed > Shader is not a DirectX11Shader");

		auto pDeviceContext = m_pDirectX11Context->GetDeviceContext();
		auto pTechnique = pDirectX11Shader->GetTechnique();

		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3DX11_TECHNIQUE_DESC techDesc{};
		pTechnique->GetDesc(&techDesc);
		for (UINT p{}; p < techDesc.Passes; ++p)
		{
			pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(indexCount, 0, 0);
		}
	}
}