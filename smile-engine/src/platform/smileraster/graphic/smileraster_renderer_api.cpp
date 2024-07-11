/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smileraster_renderer_api.h"
#include "smile_engine/core/window/window.h"

namespace smile::graphic
{
    SmileRasterRendererAPI::~SmileRasterRendererAPI()
    {
        delete m_pSmileRasterDevice;
        delete m_pSmileRasterContext;

        SelectObject( m_HDC, m_BitmapOld );
        DeleteObject( m_BitmapOld );

        DeleteObject( m_Bitmap );
        DeleteDC( m_HDC );
    }

    void SmileRasterRendererAPI::Initialize( window::Window *pWindow )
    {
        m_pWindow = pWindow;

        m_pSmileRasterContext = new SmileRasterContext{};
        m_pSmileRasterDevice = new SmileRasterDevice{ m_pSmileRasterContext->m_pDeviceContext };

        Uint32 width = m_pWindow->GetWidth();
        Uint32 height = m_pWindow->GetHeight();
        HWND handle = static_cast< HWND >( m_pWindow->GetNativeWindow() );

        m_BitmapInfo.bmiHeader.biBitCount = sizeof( Uint8 ) * 8 * 3;
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
        SM_ASSERT( m_Bitmap, "SmileRasterContext::Initialize > Failed to create BitmapDIB" );

        m_BitmapOld = static_cast< HBITMAP >( SelectObject( m_HDC, m_Bitmap ) );

        memset( m_pColorBuffer, 0, sizeof( uint8_t ) * width * height * 3 );

        m_Framebuffer = m_pSmileRasterContext->m_pDeviceContext->CreateFramebuffer(
            m_pColorBuffer, width, height, Raster::ColorbufferFormat::eRGB );
        m_pSmileRasterContext->m_pDeviceContext->BindFramebuffer( m_Framebuffer );
    }

    void SmileRasterRendererAPI::DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader )
    {
        m_pSmileRasterContext->m_pDeviceContext->DrawIndexed( indexCount );
    }

    void SmileRasterRendererAPI::Present()
    {
        Uint32 width = m_pWindow->GetWidth();
        Uint32 height = m_pWindow->GetHeight();
        HWND handle = static_cast< HWND >( m_pWindow->GetNativeWindow() );

        HDC hDC = GetDC( handle );
        BitBlt( hDC, 0, 0, width, height, m_HDC, 0, 0, SRCCOPY );
        ReleaseDC( handle, hDC );
    }

    void SmileRasterRendererAPI::Clear()
    {
        m_pSmileRasterContext->m_pDeviceContext->Clear( m_Framebuffer, m_ClearColor, true );
    }

    void SmileRasterRendererAPI::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_BitmapInfo.bmiHeader.biWidth = width;
        m_BitmapInfo.bmiHeader.biHeight = -static_cast< int >( height );
        m_BitmapInfo.bmiHeader.biSizeImage = width * height * 3;

        m_Bitmap = CreateDIBSection(
            m_HDC, &m_BitmapInfo, DIB_RGB_COLORS, reinterpret_cast< void ** >( &m_pColorBuffer ), NULL, 0 );
        SM_ASSERT( m_Bitmap, "SmileRasterRendererAPI::ResizeWindow > Failed to create BitmapDIB" );

        m_BitmapOld = static_cast< HBITMAP >( SelectObject( m_HDC, m_Bitmap ) );
        memset( m_pColorBuffer, 0, width * height * 3 );

        m_pSmileRasterContext->m_pDeviceContext->Resize( m_Framebuffer, width, height, m_pColorBuffer );
    }
}