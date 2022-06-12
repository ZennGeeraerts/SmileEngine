#pragma once
#include "smile_engine/renderer/graphics_context.h"
#include <DeviceContext.cuh>

namespace smile
{
    class Window;
}

namespace smile::renderer
{
    class SmileRasterContext final : public GraphicsContext
    {
      public:
        SmileRasterContext( Window *window );
        virtual ~SmileRasterContext();

        SmileRasterContext( const SmileRasterContext & ) = delete;
        SmileRasterContext( SmileRasterContext && ) = delete;
        SmileRasterContext &operator=( const SmileRasterContext & ) = delete;
        SmileRasterContext &operator=( SmileRasterContext && ) = delete;

        virtual void init() override;
        virtual void present() override;

        Raster::DeviceContext *getDeviceContext() const
        {
            return deviceContext;
        }

      private:
        Window *window = nullptr;

        Raster::DeviceContext *deviceContext = nullptr;

        HDC hdc = nullptr;
        HBITMAP bitmap = nullptr;
        HBITMAP bitmapOld = nullptr;
        BITMAPINFO bitmapInfo{};

        Uint8 *colorBuffer{};
        Raster::BufferID framebuffer = SMR_INVALID_BUFFER_ID;

        friend class SmileRasterRendererAPI;
    };
}
