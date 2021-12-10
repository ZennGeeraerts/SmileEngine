#include "smpch.h"
#include "SmileRasterContext.h"

#include "SmileEngine/Core/Window.h"
#include "SmileEngine/Core/Logger.h"

namespace Smile
{
	SmileRasterContext::SmileRasterContext(Window* pWindow)
		: m_pWindow{ pWindow }
	{

	}

	SmileRasterContext::~SmileRasterContext()
	{
		delete m_pDeviceContext;

		SelectObject(m_HDC, m_BitmapOld);
		DeleteObject(m_BitmapOld);

		DeleteObject(m_Bitmap);
		DeleteDC(m_HDC);
	}

	void SmileRasterContext::Init()
	{
		uint32_t width = m_pWindow->GetWidth();
		uint32_t height = m_pWindow->GetHeight();
		HWND handle = static_cast<HWND>(m_pWindow->GetNativeWindow());

		m_pDeviceContext = new SmileRasterDeviceContext{ width, height };

		BITMAPINFO bmpInfo{};
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

		HDC hDC = GetDC(handle);
		m_HDC = CreateCompatibleDC(hDC);
		ReleaseDC(handle, hDC);

		m_Bitmap = CreateDIBSection(m_HDC, &bmpInfo, DIB_RGB_COLORS, reinterpret_cast<void**>(&m_pScreenBuffer), NULL, 0);
		SM_ASSERT(m_Bitmap, "SmileRasterContext::Init > Failed to create BitmapDIB");

		m_BitmapOld = static_cast<HBITMAP>(SelectObject(m_HDC, m_Bitmap));
		memset(m_pScreenBuffer, 0, width * height * 3);
	}

	void SmileRasterContext::Present()
	{
		uint32_t width = m_pWindow->GetWidth();
		uint32_t height = m_pWindow->GetHeight();
		HWND handle = static_cast<HWND>(m_pWindow->GetNativeWindow());

		HDC hDC = GetDC(handle);
		BitBlt(hDC, 0, 0, width, height, m_HDC, 0, 0, SRCCOPY);
		ReleaseDC(handle, hDC);
	}
}