#include "smpch.h"
#include "smileraster_renderer_api.h"

#include "smile_engine/core/application.h"

namespace Smile::Graphic
{
    void SmileRasterRendererAPI::Initialize()
    {
        m_pWindow = &Application::GetInstance().GetWindow();
        m_pSmileRasterContext = static_cast< SmileRasterContext * >( m_pWindow->GetGraphicsContext() );
        SM_ASSERT( m_pSmileRasterContext, "SmileRasterRendererAPI > RenderingContext is not a SmileRasterContext" );
    }

    void SmileRasterRendererAPI::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_pSmileRasterContext->m_BitmapInfo.bmiHeader.biWidth = width;
        m_pSmileRasterContext->m_BitmapInfo.bmiHeader.biHeight = -static_cast< int >( height );
        m_pSmileRasterContext->m_BitmapInfo.bmiHeader.biSizeImage = width * height * 3;

        m_pSmileRasterContext->m_Bitmap = CreateDIBSection( m_pSmileRasterContext->m_HDC,
            &m_pSmileRasterContext->m_BitmapInfo,
            DIB_RGB_COLORS,
            reinterpret_cast< void ** >( &m_pSmileRasterContext->m_pColorBuffer ),
            NULL,
            0 );
        SM_ASSERT(
            m_pSmileRasterContext->m_Bitmap, "SmileRasterRendererAPI::ResizeWindow > Failed to create BitmapDIB" );

        m_pSmileRasterContext->m_BitmapOld =
            static_cast< HBITMAP >( SelectObject( m_pSmileRasterContext->m_HDC, m_pSmileRasterContext->m_Bitmap ) );
        memset( m_pSmileRasterContext->m_pColorBuffer, 0, width * height * 3 );

        m_pSmileRasterContext->m_pDeviceContext->Resize(
            m_pSmileRasterContext->m_Framebuffer, width, height, m_pSmileRasterContext->m_pColorBuffer );
    }

    void SmileRasterRendererAPI::SetClearColor( const DirectX::XMFLOAT4 &color )
    {
        m_ClearColor = color;
    }

    void SmileRasterRendererAPI::Clear()
    {
        m_pSmileRasterContext->m_pDeviceContext->Clear( m_pSmileRasterContext->m_Framebuffer, m_ClearColor, true );
    }

    void SmileRasterRendererAPI::DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader )
    {
        m_pSmileRasterContext->m_pDeviceContext->DrawIndexed( indexCount );
    }
}