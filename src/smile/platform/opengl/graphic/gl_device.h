/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/graphics_device.h"

namespace smile::graphic
{
    class GLDevice final : public GraphicsDevice
    {
      public:
        GLDevice() = default;

        void Initialize( GraphicsContext *pContext ) override;

        std::shared_ptr< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &desc ) const override;
        std::shared_ptr< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &desc ) const override;
        std::shared_ptr< Shader > CreateShader( const std::string &name,
            const std::string &vertexShaderSrc,
            const std::string &fragmentShaderSrc ) const override;
        std::shared_ptr< Shader > CreateShader( const std::filesystem::path &filePath ) const override;
        std::shared_ptr< VertexArrayObject > CreateVertexArrayObject(
            const std::vector< std::shared_ptr< VertexBuffer > > &pVertexBuffers,
            std::shared_ptr< IndexBuffer > pIndexBuffer ) const override;
        std::shared_ptr< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &desc ) const override;

        void
        ResizeFramebuffer( std::shared_ptr< Framebuffer > pFramebuffer, Uint32 width, Uint32 height ) const override;

      private:
        GraphicsContext *m_pContext = nullptr;
    };
}