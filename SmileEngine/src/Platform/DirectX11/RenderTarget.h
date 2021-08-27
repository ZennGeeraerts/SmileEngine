#pragma once

#include "SmileEngine/Logger.h"

namespace Smile
{
	struct RENDERTARGET_DESC
	{
	public:
		int Width;
		int Height;

		//Enable this if you want to create a DepthStencilBuffer
		bool bEnableDepthBuffer;
		//Enable this if you want to use the DepthStencilBuffer
		//as a ShaderResourceView (Texture in a shader)
		bool bEnableDepthSRV;
		//Enable this if you want to create a RenderTarget (color)
		bool bEnableColorBuffer;
		//Enable this if you want to use the RenderTarget
		//as a ShaderResourceView (Texture in a shader)
		bool bEnableColorSRV;
		//Generate MipMaps (ColorSRV Only)
		bool bGenerateMipMapsColor;

		//Depth buffer format (DepthStencil)
		DXGI_FORMAT DepthFormat;
		//Color buffer format (Rendertarget)
		DXGI_FORMAT ColorFormat;

		//Optional: Supply one of the buffers
		ID3D11Texture2D* pColor;
		bool bColorBufferSupplied;
		ID3D11Texture2D* pDepth;
		bool bDepthBufferSupplied;

		RENDERTARGET_DESC() 
			: Width(-1)
			, Height(-1)
			, bEnableDepthBuffer{ true }
			, bEnableDepthSRV{ false }
			, bEnableColorBuffer{ true }
			, bEnableColorSRV{ false }
			, bGenerateMipMapsColor{ false }
			, bColorBufferSupplied{ false }
			, bDepthBufferSupplied{ false }
#pragma warning(push)
#pragma warning(disable:26812)
			, DepthFormat{ DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT }
			, ColorFormat{ DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM }
#pragma warning(pop)
			, pColor{ nullptr }
			, pDepth{ nullptr }
		{}

		void IsValid()
		{
			if (!(pColor || pDepth) && (Width <= 0 || Height <= 0))
				SM_LOG_ERROR("RenderTarget::IsValid() > Invalid Width and/or Height!");

			if (!(bEnableColorBuffer || bEnableDepthBuffer))
				SM_LOG_ERROR("RenderTarget::IsValid() > Rendertarget must contain at least one buffer! (Color AND/OR Depth)");

			if (bEnableDepthSRV && !bEnableDepthBuffer)
			{
				bEnableDepthBuffer = true;
				SM_LOG_WARNING("RenderTarget::IsValid() > Forced \'bEnableDepthBuffer\' flag because \'bEnableDepthSRV\' is enabled by user.");
			}

			if (bEnableColorSRV && !bEnableColorBuffer)
			{
				bEnableColorBuffer = true;
				SM_LOG_WARNING("RenderTarget::IsValid() > Forced \'bEnableColorBuffer\' flag because \'bEnableColorSRV\' is enabled by user.");
			}
		}
	};

	class RenderTarget final
	{
	public:
		RenderTarget(ID3D11Device* pDevice);
		~RenderTarget();

		HRESULT Create(RENDERTARGET_DESC desc);
		inline ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }
		inline bool HasRenderTargetView() const { return m_pRenderTargetView; }
		inline ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView; }
		ID3D11ShaderResourceView* GetShaderResourceView() const;
		ID3D11ShaderResourceView* GetDepthShaderResourceView() const;

		inline bool HasColorSRV() const { return m_Desc.bEnableColorSRV; }
		inline bool HasDepthSRV() const { return m_Desc.bEnableDepthSRV; }

	private:
		RENDERTARGET_DESC m_Desc;
		ID3D11Device* m_pDevice = nullptr;
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
		ID3D11ShaderResourceView* m_pColorShaderResourceView = nullptr;
		ID3D11ShaderResourceView* m_pDepthShaderResourceView = nullptr;
		ID3D11Texture2D* m_pColor = nullptr;
		ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
		ID3D11Texture2D* m_pDepth = nullptr;

		HRESULT CreateColor();
		HRESULT CreateDepth();
		static DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT initFormat);
		static DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT initFormat);
		void CleanUp();

		bool m_bColorBufferSupplied = false;
		bool m_bDepthBufferSupplied = false;
	};
}

