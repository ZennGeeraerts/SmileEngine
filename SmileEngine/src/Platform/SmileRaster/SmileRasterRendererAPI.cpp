#include "smpch.h"
#include "SmileRasterRendererAPI.h"

#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	void SmileRasterRendererAPI::Initialize()
	{
		m_pWindow = &SmileGame::GetInstance().GetWindow();
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(m_pWindow->GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterRendererAPI > RenderingContext is not a SmileRasterContext");
	}

	void SmileRasterRendererAPI::ResizeWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		/*BITMAPINFO bmpInfo{};
		bmpInfo.bmiHeader.biBitCount = 24;
		bmpInfo.bmiHeader.biClrImportant = 0;
		bmpInfo.bmiHeader.biClrUsed = 0;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biWidth = width;
		bmpInfo.bmiHeader.biHeight = -static_cast<int>(height);
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFO);
		bmpInfo.bmiHeader.biSizeImage = width * height * 3;
		bmpInfo.bmiHeader.biXPelsPerMeter = 0;
		bmpInfo.bmiHeader.biYPelsPerMeter = 0;

		m_pSmileRasterContext->m_Bitmap = CreateDIBSection(m_pSmileRasterContext->m_DeviceContext, &bmpInfo, DIB_RGB_COLORS, reinterpret_cast<void**>(&m_pSmileRasterContext->m_pScreenBuffer), NULL, 0);
		SM_ASSERT(m_pSmileRasterContext->m_Bitmap, "SmileRasterContext::Init > Failed to create BitmapDIB");

		m_pSmileRasterContext->m_BitmapOld = static_cast<HBITMAP>(SelectObject(m_pSmileRasterContext->m_DeviceContext, m_pSmileRasterContext->m_Bitmap));
		memset(m_pSmileRasterContext->m_pScreenBuffer, 0, width * height * 3);*/
	}

	void SmileRasterRendererAPI::SetClearColor(const DirectX::XMFLOAT4& color)
	{
		m_ClearColor = color;
	}

	void SmileRasterRendererAPI::Clear()
	{
		m_pSmileRasterContext->m_pDeviceContext->Clear(DirectX::XMFLOAT3{ m_ClearColor.x, m_ClearColor.y, m_ClearColor.z });
	}

	void SmileRasterRendererAPI::DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader)
	{
		
	}
}