/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/graphics_device.h"
#include <device_context.cuh>

namespace smile::graphic
{
    class SmileRasterDevice final : public GraphicsDevice
    {
      public:
        SmileRasterDevice( Raster::DeviceContext *pContext );

        virtual void *GetInternal() const
        {
            return m_pDeviceContext;
        }

        Ref< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc ) override;
        Ref< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc ) override;
        Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) override;
        Ref< Shader > CreateShader( const std::string &assetFile, const std::string &techniqueName = "" ) override;
        Ref< Texture2D > CreateTexture2D( const std::string &filePath ) override;
        Ref< TextureCube > CreateTextureCube( const std::string &filePath ) override;
        Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) override;
        Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) override;

        void InvalidateFramebuffer( const Ref< Framebuffer > &pFramebuffer ) override;
        void ResizeFramebuffer( const Ref< Framebuffer > &pFramebuffer, Uint32 width, Uint32 height ) override;

      private:
        Raster::DeviceContext *m_pDeviceContext;
    };
}