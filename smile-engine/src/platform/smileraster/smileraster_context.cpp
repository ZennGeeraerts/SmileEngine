#include "smpch.h"
#include "smileraster_context.h"

#include "smile_engine/core/window.h"
#include "smile_engine/core/logger.h"

namespace smile::renderer
{
    SmileRasterContext::SmileRasterContext( Window *window ) : window{ window }
    {
    }

    SmileRasterContext::~SmileRasterContext()
    {
        delete deviceContext;

        SelectObject( hdc, bitmapOld );
        DeleteObject( bitmapOld );

        DeleteObject( bitmap );
        DeleteDC( hdc );
    }

    void SmileRasterContext::init()
    {
        Uint32 width = window->getWidth();
        Uint32 height = window->getHeight();
        HWND handle = static_cast< HWND >( window->getNativeWindow() );

        bitmapInfo.bmiHeader.biBitCount = sizeof( Uint8 ) * 8 * 3;
        bitmapInfo.bmiHeader.biClrImportant = 0;
        bitmapInfo.bmiHeader.biClrUsed = 0;
        bitmapInfo.bmiHeader.biCompression = BI_RGB;
        bitmapInfo.bmiHeader.biWidth = width;
        bitmapInfo.bmiHeader.biHeight = -static_cast< int >( height );
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biSize = sizeof( BITMAPINFO );
        bitmapInfo.bmiHeader.biSizeImage = width * height * 3;
        bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
        bitmapInfo.bmiHeader.biYPelsPerMeter = 0;

        HDC hDC = GetDC( handle );
        hdc = CreateCompatibleDC( hDC );
        ReleaseDC( handle, hDC );

        bitmap = CreateDIBSection(
            hdc, &bitmapInfo, DIB_RGB_COLORS, reinterpret_cast< void ** >( &colorBuffer ), NULL, 0 );
        SM_ASSERT( bitmap, "SmileRasterContext::Init > Failed to create BitmapDIB" );

        bitmapOld = static_cast< HBITMAP >( SelectObject( hdc, bitmap ) );

        memset( colorBuffer, 0, sizeof( uint8_t ) * width * height * 3 );

        Raster::RenderConfig renderConfig{};
        deviceContext = new Raster::DeviceContext{ renderConfig };
        framebuffer =
            deviceContext->CreateFramebuffer( colorBuffer, width, height, Raster::ColorbufferFormat::eRGB );
        deviceContext->BindFramebuffer( framebuffer );
    }

    void SmileRasterContext::present()
    {
        Uint32 width = window->getWidth();
        Uint32 height = window->getHeight();
        HWND handle = static_cast< HWND >( window->getNativeWindow() );

        HDC hDC = GetDC( handle );
        BitBlt( hDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY );
        ReleaseDC( handle, hDC );
    }
}