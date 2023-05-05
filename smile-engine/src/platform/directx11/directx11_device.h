/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/graphic/graphics_device.h"

#include <d3d11.h>

namespace smile::graphic
{
    class DirectX11Device final : public GraphicsDevice
    {
      public:
        DirectX11Device() = default;
        ~DirectX11Device();

        void Initialize( GraphicsContext *pGraphicsContext ) override;

        void *GetInternal() const override
        {
            return m_pInternal;
        }

        Ref< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc ) override;
        Ref< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc ) override;
        Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) override;
        Ref< Shader > CreateShader( const std::string &assetFile, const std::string &techniqueName = "" ) override;
        Ref< Texture2D > CreateTexture2D( const std::string &filePath ) override;
        Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) override;
        Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) override;

        void InvalidateFramebuffer( const Ref< Framebuffer > &pFramebuffer ) override;
        void ResizeFramebuffer( const Ref< Framebuffer > &pFramebuffer, Uint32 width, Uint32 height ) override;

      private:
        ID3D11Device *m_pInternal = nullptr;

        friend class DirectX11RendererAPI;
    };
}
