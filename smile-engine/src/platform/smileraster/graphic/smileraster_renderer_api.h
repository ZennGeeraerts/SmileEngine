/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api.h"
#include "smileraster_context.h"
#include "smile_raster_device.h"

namespace smile::graphic
{
    class SmileRasterRendererAPI final : public RendererAPI
    {
      public:
        SmileRasterRendererAPI() = default;
        ~SmileRasterRendererAPI();

        void Initialize( window::Window *pWindow ) override;

        void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) override;
        void Present() override;
        void Clear() override;
        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;

      private:
        window::Window *m_pWindow = nullptr;
        SmileRasterDevice *m_pSmileRasterDevice = nullptr;
        SmileRasterContext *m_pSmileRasterContext = nullptr;

        HDC m_HDC = nullptr;
        HBITMAP m_Bitmap = nullptr;
        HBITMAP m_BitmapOld = nullptr;
        BITMAPINFO m_BitmapInfo{};

        Uint8 *m_pColorBuffer{};
        Raster::BufferID m_Framebuffer = SMR_INVALID_BUFFER_ID;
    };
}
