#include "smpch.h"
#include "smileraster_renderer_api.h"

#include "smile_engine/core/application.h"

namespace smile::graphic
{
    void SmileRasterRendererAPI::initialize()
    {
        window = &Application::getInstance().getWindow();
        smileRasterContext = static_cast< SmileRasterContext * >( window->getGraphicsContext() );
        SM_ASSERT( smileRasterContext, "SmileRasterRendererAPI > RenderingContext is not a SmileRasterContext" );
    }

    void SmileRasterRendererAPI::resizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        smileRasterContext->bitmapInfo.bmiHeader.biWidth = width;
        smileRasterContext->bitmapInfo.bmiHeader.biHeight = -static_cast< int >( height );
        smileRasterContext->bitmapInfo.bmiHeader.biSizeImage = width * height * 3;

        smileRasterContext->bitmap = CreateDIBSection( smileRasterContext->hdc,
            &smileRasterContext->bitmapInfo,
            DIB_RGB_COLORS,
            reinterpret_cast< void ** >( &smileRasterContext->colorBuffer ),
            NULL,
            0 );
        SM_ASSERT(
            smileRasterContext->bitmap, "SmileRasterRendererAPI::resizeWindow > Failed to create BitmapDIB" );

        smileRasterContext->bitmapOld =
            static_cast< HBITMAP >( SelectObject( smileRasterContext->hdc, smileRasterContext->bitmap ) );
        memset( smileRasterContext->colorBuffer, 0, width * height * 3 );

        smileRasterContext->deviceContext->Resize(
            smileRasterContext->framebuffer, width, height, smileRasterContext->colorBuffer );
    }

    void SmileRasterRendererAPI::setClearColor( const DirectX::XMFLOAT4 &color )
    {
        clearColor = color;
    }

    void SmileRasterRendererAPI::clear()
    {
        smileRasterContext->deviceContext->Clear( smileRasterContext->framebuffer, clearColor, true );
    }

    void SmileRasterRendererAPI::drawIndexed( Uint32 indexCount, const Ref< Shader > &shader )
    {
        smileRasterContext->deviceContext->DrawIndexed( indexCount );
    }
}