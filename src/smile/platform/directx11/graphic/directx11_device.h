/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_api/graphics_device.h"

#include <d3d11.h>

namespace smile::graphic
{
    class GraphicsContext;

    class DirectX11Device final : public GraphicsDevice
    {
      public:
        DirectX11Device( GraphicsContext *pContext );
        ~DirectX11Device();

        void *GetInternal() const override
        {
            return m_pInternal;
        }

        memory::Ref< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc ) override;
        memory::Ref< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc ) override;
        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) override;
        memory::Ref< Texture > CreateTexture2D( const std::string &filePath ) override;
        memory::Ref< Texture > CreateTextureCube( const std::string &filePath ) override;
        memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) override;
        memory::Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) override;

        void InvalidateFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) override;

      private:
        ID3D11Device *m_pInternal = nullptr;
    };
}
