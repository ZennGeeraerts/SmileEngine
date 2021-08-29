#include "smpch.h"
#include "DirectX11RendererAPI.h"

#include "DirectX11Context.h"
#include "DirectX11Shader.h"

namespace Smile
{
	void DirectX11RendererAPI::SetClearColor(const DirectX::XMFLOAT4& color)
	{
		m_ClearColor = color;
	}

	void DirectX11RendererAPI::Clear(RenderingContext* pRenderingContext)
	{
		auto pDirectX11Context = static_cast<DirectX11Context*>(pRenderingContext);
		SM_ASSERT(pDirectX11Context, "DirectX11RendererAPI::Clear > RenderingContext is not a DirectX11Context");

		auto pDeviceContext = pDirectX11Context->GetDeviceContext();
		auto pRenderTargetView = pDirectX11Context->GetRenderTargetView();
		auto pDepthStencilView = pDirectX11Context->GetDepthStencilView();

		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		const float* pClearColor = reinterpret_cast<const float*>(&m_ClearColor);
		pDeviceContext->ClearRenderTargetView(pRenderTargetView, pClearColor);
		pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void DirectX11RendererAPI::DrawIndexed(RenderingContext* pRenderingContext, int32_t indexCount, const std::shared_ptr<Shader>& pShader)
	{
		auto pDirectX11Context = static_cast<DirectX11Context*>(pRenderingContext);
		SM_ASSERT(pDirectX11Context, "DirectX11RendererAPI::DrawIndexed > RenderingContext is not a DirectX11Context");

		auto pDeviceContext = pDirectX11Context->GetDeviceContext();

		auto pDirectX11Shader = static_cast<DirectX11Shader*>(pShader.get());
		SM_ASSERT(pDirectX11Shader, "DirectX11RendererAPI::DrawIndexed > Shader is not a DirectX11Shader");

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