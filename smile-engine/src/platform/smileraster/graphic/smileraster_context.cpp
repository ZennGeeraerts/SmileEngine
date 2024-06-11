/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smileraster_context.h"

#include "smile_engine/core/window/window.h"
#include "smile_engine/common/logger/logger.h"

namespace smile::graphic
{
    SmileRasterContext::SmileRasterContext( window::Window *pWindow ) : m_pWindow{ pWindow }
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

    void SmileRasterContext::Initialize( GraphicsDevice *pGraphicsDevice )
    {
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

        Raster::RenderConfig renderConfig{};
        m_pDeviceContext = new Raster::DeviceContext{ renderConfig };
        m_Framebuffer =
            m_pDeviceContext->CreateFramebuffer( m_pColorBuffer, width, height, Raster::ColorbufferFormat::eRGB );
        m_pDeviceContext->BindFramebuffer( m_Framebuffer );
    }

    void SmileRasterContext::Present()
    {
        Uint32 width = m_pWindow->GetWidth();
        Uint32 height = m_pWindow->GetHeight();
        HWND handle = static_cast< HWND >( m_pWindow->GetNativeWindow() );

        HDC hDC = GetDC( handle );
        BitBlt( hDC, 0, 0, width, height, m_HDC, 0, 0, SRCCOPY );
        ReleaseDC( handle, hDC );
    }

    void SmileRasterContext::BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const
    {
        Raster::BufferID bufferID = reinterpret_cast< Raster::BufferID >( pVertexBuffer->GetInternal() );
        m_pDeviceContext->BindVertexBuffer( bufferID, pVertexBuffer->Stride );
    }

    void SmileRasterContext::UnbindVertexBuffer() const
    {
        m_pDeviceContext->BindVertexBuffer( SMR_INVALID_BUFFER_ID, 0 );
    }

    void SmileRasterContext::BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const
    {
        Raster::BufferID bufferID = reinterpret_cast< Raster::BufferID >( pIndexBuffer->GetInternal() );
        m_pDeviceContext->BindIndexBuffer( bufferID );
    }

    void SmileRasterContext::UnbindIndexBuffer() const
    {
        m_pDeviceContext->BindIndexBuffer( SMR_INVALID_BUFFER_ID );
    }
}