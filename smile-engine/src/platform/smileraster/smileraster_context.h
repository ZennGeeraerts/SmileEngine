/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/graphic/graphics_context.h"

#include <DeviceContext.cuh>

namespace smile
{
    class Window;
}

namespace smile::graphic
{
    class SmileRasterContext final : public GraphicsContext
    {
      public:
        SmileRasterContext( Window *pWindow );
        virtual ~SmileRasterContext();

        SmileRasterContext( const SmileRasterContext & ) = delete;
        SmileRasterContext( SmileRasterContext && ) = delete;
        SmileRasterContext &operator=( const SmileRasterContext & ) = delete;
        SmileRasterContext &operator=( SmileRasterContext && ) = delete;

        virtual void Initialize( GraphicsDevice *pGraphicsDevice ) override;
        virtual void Present() override;

        Raster::DeviceContext *GetDeviceContext() const
        {
            return m_pDeviceContext;
        }

        void BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const override;
        void UnbindVertexBuffer() const override;
        void BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const override;
        void UnbindIndexBuffer() const override;

      private:
        Window *m_pWindow = nullptr;

        Raster::DeviceContext *m_pDeviceContext = nullptr;

        HDC m_HDC = nullptr;
        HBITMAP m_Bitmap = nullptr;
        HBITMAP m_BitmapOld = nullptr;
        BITMAPINFO m_BitmapInfo{};

        Uint8 *m_pColorBuffer{};
        Raster::BufferID m_Framebuffer = SMR_INVALID_BUFFER_ID;

        friend class SmileRasterRendererAPI;
    };
}
