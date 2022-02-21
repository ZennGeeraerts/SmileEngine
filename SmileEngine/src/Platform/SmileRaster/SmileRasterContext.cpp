#include "smpch.h"
#include "SmileRasterContext.h"

#include "SmileEngine/Core/Window.h"
#include "SmileEngine/Core/Logger.h"

namespace smile
{
    SmileRasterContext::SmileRasterContext( Window *pWindow ) : m_pWindow{ pWindow }
    {
    }

    SmileRasterContext::~SmileRasterContext()
    {
        delete m_pDeviceContext;

        SelectObject( m_HDC, m_BitmapOld );
        DeleteObject( m_BitmapOld );

        DeleteObject( m_Bitmap );
        DeleteDC( m_HDC );
    }

    void SmileRasterContext::Init()
    {
        uint32_t width = m_pWindow->GetWidth();
        uint32_t height = m_pWindow->GetHeight();
        HWND handle = static_cast< HWND >( m_pWindow->GetNativeWindow() );

        m_BitmapInfo.bmiHeader.biBitCount = sizeof( uint8_t ) * 8 * 3;
        m_BitmapInfo.bmiHeader.biClrImportant = 0;
        m_BitmapInfo.bmiHeader.biClrUsed = 0;
        m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
        m_BitmapInfo.bmiHeader.biWidth = width;
        m_BitmapInfo.bmiHeader.biHeight = -static_cast< int >( height );
        m_BitmapInfo.bmiHeader.biPlanes = 1;
        m_BitmapInfo.bmiHeader.biSize = sizeof( BITMAPINFO );
        m_BitmapInfo.bmiHeader.biSizeImage = width * height * 3;
        m_BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
        m_BitmapInfo.bmiHeader.biYPelsPerMeter = 0;

        HDC hDC = GetDC( handle );
        m_HDC = CreateCompatibleDC( hDC );
        ReleaseDC( handle, hDC );

        m_Bitmap = CreateDIBSection(
            m_HDC, &m_BitmapInfo, DIB_RGB_COLORS, reinterpret_cast< void ** >( &m_pColorBuffer ), NULL, 0 );
        SM_ASSERT( m_Bitmap, "SmileRasterContext::Init > Failed to create BitmapDIB" );

        m_BitmapOld = static_cast< HBITMAP >( SelectObject( m_HDC, m_Bitmap ) );

        memset( m_pColorBuffer, 0, sizeof( uint8_t ) * width * height * 3 );

        Raster::RenderConfig renderConfig{};
        m_pDeviceContext = new Raster::DeviceContext{ renderConfig };
        m_Framebuffer =
            m_pDeviceContext->CreateFramebuffer( m_pColorBuffer, width, height, Raster::ColorbufferFormat::eRGB );
        m_pDeviceContext->BindFramebuffer( m_Framebuffer );
    }

    void SmileRasterContext::Present()
    {
        uint32_t width = m_pWindow->GetWidth();
        uint32_t height = m_pWindow->GetHeight();
        HWND handle = static_cast< HWND >( m_pWindow->GetNativeWindow() );

        HDC hDC = GetDC( handle );
        BitBlt( hDC, 0, 0, width, height, m_HDC, 0, 0, SRCCOPY );
        ReleaseDC( handle, hDC );
    }
}