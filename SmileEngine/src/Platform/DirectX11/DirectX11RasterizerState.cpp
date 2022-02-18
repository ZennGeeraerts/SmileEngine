#include "smpch.h"
#include "DirectX11RasterizerState.h"

#include "SmileEngine/Core/Application.h"
#include "SmileEngine/Core/Logger.h"

namespace Smile
{
	static D3D11_CULL_MODE CullModeToDirectXType(CullMode cullMode)
	{
		switch (cullMode)
		{
		case CullMode::eNone: return D3D11_CULL_NONE;
		case CullMode::eFront: return D3D11_CULL_FRONT;
		case CullMode::eBack: return D3D11_CULL_BACK;
		default: return D3D11_CULL_NONE;
		}
	}

	static D3D11_FILL_MODE FillModeToDirectXType(FillMode fillMode)
	{
		switch (fillMode)
		{
		case FillMode::eSolid: return D3D11_FILL_SOLID;
		case FillMode::eWireFrame: return D3D11_FILL_WIREFRAME;
		default: return D3D11_FILL_SOLID;
		}
	}

	DirectX11RasterizerState::DirectX11RasterizerState(const RasterizerStateData& rasterizerStateData)
	{
		m_pDirectX11Context = static_cast<DirectX11Context*>(Application::GetInstance().GetWindow().GetRenderingContext());

		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.CullMode = CullModeToDirectXType(rasterizerStateData.CullMode);
		rasterizerDesc.FillMode = FillModeToDirectXType(rasterizerStateData.FillMode);
		rasterizerDesc.DepthClipEnable = rasterizerStateData.bDepthClipEnable;

		HRESULT result = m_pDirectX11Context->GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
		if (FAILED(result))
			SM_LOG_ERROR("DirectX11RasterizerState::DirectX11RasterizerState > Failed to create rasterizer state");
	}

	DirectX11RasterizerState::~DirectX11RasterizerState()
	{
		SAFE_RELEASE(m_pRasterizerState);
	}

	void DirectX11RasterizerState::Bind() const
	{
		m_pDirectX11Context->GetDeviceContext()->RSSetState(m_pRasterizerState);
	}

	void DirectX11RasterizerState::Unbind() const
	{
		m_pDirectX11Context->GetDeviceContext()->RSSetState(nullptr);
	}
}