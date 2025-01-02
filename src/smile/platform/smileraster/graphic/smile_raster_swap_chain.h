/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/engine/graphic/swap_chain.h"
#include <device_context.cuh>

namespace smile::graphic
{
    class GraphicsDevice;
    class GraphicsContext;

    class SmileRasterSwapChain final : public SwapChain
    {
      public:
        SmileRasterSwapChain( Raster::DeviceContext *pDeviceContext, window::Window *pWindow );
        ~SmileRasterSwapChain();

        void Present() override;
        void Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;

        Raster::BufferID GetFramebuffer() const
        {
            return m_Framebuffer;
        }

      private:
        Raster::DeviceContext *m_pDeviceContext;

        HDC m_HDC = nullptr;
        HBITMAP m_Bitmap = nullptr;
        HBITMAP m_BitmapOld = nullptr;
        BITMAPINFO m_BitmapInfo{};

        Uint8 *m_pColorBuffer{};
        Raster::BufferID m_Framebuffer = SMR_INVALID_BUFFER_ID;
    };
}