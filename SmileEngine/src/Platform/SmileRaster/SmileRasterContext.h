#pragma once
#include "SmileEngine/Renderer/GraphicsContext.h"
#include <DeviceContext.cuh>

namespace smile
{
    class Window;

    class SmileRasterContext final : public GraphicsContext
    {
      public:
        SmileRasterContext( Window *pWindow );
        virtual ~SmileRasterContext();

        SmileRasterContext( const SmileRasterContext & ) = delete;
        SmileRasterContext( SmileRasterContext && ) = delete;
        SmileRasterContext &operator=( const SmileRasterContext & ) = delete;
        SmileRasterContext &operator=( SmileRasterContext && ) = delete;

        virtual void Init() override;
        virtual void Present() override;

        Raster::DeviceContext *GetDeviceContext() const
        {
            return m_pDeviceContext;
        }

      private:
        Window *m_pWindow = nullptr;

        Raster::DeviceContext *m_pDeviceContext = nullptr;

        HDC m_HDC = nullptr;
        HBITMAP m_Bitmap = nullptr;
        HBITMAP m_BitmapOld = nullptr;
        BITMAPINFO m_BitmapInfo{};

        uint8_t *m_pColorBuffer{};
        Raster::BufferID m_Framebuffer = SMR_INVALID_BUFFER_ID;

        friend class SmileRasterRendererAPI;
    };
}
