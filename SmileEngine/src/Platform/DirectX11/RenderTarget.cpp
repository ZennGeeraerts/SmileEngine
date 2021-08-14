#include "smpch.h"
#include "RenderTarget.h"

namespace Smile
{
	RenderTarget::RenderTarget(ID3D11Device* pDevice) 
		: m_pDevice(pDevice)
	{}

	RenderTarget::~RenderTarget()
	{
		CleanUp();
	}

	void RenderTarget::CleanUp()
	{
		SAFE_RELEASE(m_pColor);
		SAFE_RELEASE(m_pDepth);
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pColorShaderResourceView);
		SAFE_RELEASE(m_pDepthShaderResourceView);
		SAFE_RELEASE(m_pDepthStencilView);
	}

	HRESULT RenderTarget::Create(RENDERTARGET_DESC desc)
	{
		CleanUp();

		//Check if input is valid
		desc.IsValid();
		m_Desc = desc;

		m_bColorBufferSupplied = m_Desc.pColor;
		m_bDepthBufferSupplied = m_Desc.pDepth;

		HRESULT result = CreateColor();
		if (FAILED(result))
			return result;

		result = CreateDepth();
		if (FAILED(result))
			return result;

		return S_OK;
	}

	HRESULT RenderTarget::CreateColor()
	{
		if (m_Desc.pColor)
		{
			m_pColor = m_Desc.pColor;

			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(texDesc));

			m_pColor->GetDesc(&texDesc);
			m_Desc.Width = texDesc.Width;
			m_Desc.Height = texDesc.Height;
			m_Desc.ColorFormat = texDesc.Format;

			m_Desc.bEnableColorSRV = (texDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE;
		}

		if (m_Desc.bEnableColorBuffer)
		{
			if (m_pColor == nullptr)
			{
				//RESOURCE
				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(textureDesc));

				textureDesc.Width = m_Desc.Width;
				textureDesc.Height = m_Desc.Height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = m_Desc.ColorFormat;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | ((m_Desc.bEnableColorSRV) ? D3D11_BIND_SHADER_RESOURCE : 0);
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = ((m_Desc.bGenerateMipMapsColor) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);

				HRESULT result = m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pColor);
				if (FAILED(result))
					return result;

				m_Desc.pColor = m_pColor;
			}

			//RENDERTARGET SRV
			auto result = m_pDevice->CreateRenderTargetView(m_pColor, nullptr, &m_pRenderTargetView);
			if (FAILED(result))
				return result;

			//SHADER SRV
			if (m_Desc.bEnableColorSRV)
			{
				result = m_pDevice->CreateShaderResourceView(m_pColor, nullptr, &m_pColorShaderResourceView);
				if (FAILED(result))
					return result;
			}
		}
		else
		{
			SAFE_RELEASE(m_pColor);
			SAFE_RELEASE(m_pRenderTargetView);
			SAFE_RELEASE(m_pColorShaderResourceView);
		}

		return S_OK;
	}

	DXGI_FORMAT RenderTarget::GetDepthResourceFormat(DXGI_FORMAT initFormat)
	{
		DXGI_FORMAT resourceFormat = {};
		switch (initFormat)
		{
		case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
			resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
			resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
			resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
			break;
		default:
			SM_ERROR("RenderTarget::GetDepthResourceFormat(...) > Format not supported!");
			break;
		}

		return resourceFormat;
	}

	DXGI_FORMAT RenderTarget::GetDepthSRVFormat(DXGI_FORMAT initFormat)
	{
		DXGI_FORMAT srvFormat = {};
		switch (initFormat)
		{
		case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
			srvFormat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
			srvFormat = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
			srvFormat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			srvFormat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;
		default:
			SM_ERROR("RenderTarget::GetDepthSRVFormat(...) > Format not supported!");
			break;
		}

		return srvFormat;
	}

	HRESULT RenderTarget::CreateDepth()
	{
		if (m_Desc.pDepth)
		{
			m_pDepth = m_Desc.pDepth;

			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(texDesc));

			m_pDepth->GetDesc(&texDesc);
			m_Desc.Width = texDesc.Width;
			m_Desc.Height = texDesc.Height;
			m_Desc.DepthFormat = texDesc.Format;
		}

		if (m_Desc.bEnableDepthBuffer)
		{
			if (m_pDepth == nullptr)
			{
				//RESOURCE
				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(textureDesc));

				textureDesc.Width = m_Desc.Width;
				textureDesc.Height = m_Desc.Height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = GetDepthResourceFormat(m_Desc.DepthFormat);
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | ((m_Desc.bEnableDepthSRV) ? D3D11_BIND_SHADER_RESOURCE : 0);
				textureDesc.MiscFlags = 0;

				auto hr = m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pDepth);
				if (FAILED(hr))
					return hr;

				m_Desc.pDepth = m_pDepth;
			}

			//DEPTHSTENCIL VIEW
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));

			descDSV.Format = m_Desc.DepthFormat;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			HRESULT result = m_pDevice->CreateDepthStencilView(m_pDepth, &descDSV, &m_pDepthStencilView);
			if (FAILED(result))
				return result;

			//SHADER SRV
			if (m_Desc.bEnableDepthSRV)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc;
				ZeroMemory(&depthSrvDesc, sizeof(depthSrvDesc));

				depthSrvDesc.Format = GetDepthSRVFormat(m_Desc.DepthFormat);
				depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				depthSrvDesc.Texture2D.MipLevels = 1;
				depthSrvDesc.Texture2D.MostDetailedMip = 0;

				result = m_pDevice->CreateShaderResourceView(m_pDepth, &depthSrvDesc, &m_pDepthShaderResourceView);
				if (FAILED(result))
					return result;
			}
		}
		else
		{
			SAFE_RELEASE(m_pColor);
			SAFE_RELEASE(m_pRenderTargetView);
			SAFE_RELEASE(m_pColorShaderResourceView);
		}

		return S_OK;
	}

	ID3D11ShaderResourceView* RenderTarget::GetShaderResourceView() const
	{
		if (!m_Desc.bEnableColorSRV)
			SM_ERROR("RenderTarget::GetShaderResourceView(...) > No COLOR SRV created during Creation. (Make sure to enable \'EnableColorSRV\' during RT Creation)");

		return m_pColorShaderResourceView;
	}

	ID3D11ShaderResourceView* RenderTarget::GetDepthShaderResourceView() const
	{
		if (!m_Desc.bEnableDepthSRV)
			SM_ERROR("RenderTarget::GetDepthShaderResourceView(...) > No DEPTH SRV created during Creation. (Make sure to enable \'EnableDepthSRV\' during RT Creation)");

		return m_pDepthShaderResourceView;
	}
}