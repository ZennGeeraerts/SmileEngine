/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/graphics_context.h"

namespace smile::graphic
{
    class GLContext final : public GraphicsContext
    {
      public:
        GLContext() = default;

        void Initialize() override;
        void Present() override;

        void BindVertexBuffer( std::shared_ptr< VertexBuffer > pVertexBuffer ) const override;
        void UnbindVertexBuffer() const override;
        void BindIndexBuffer( std::shared_ptr< IndexBuffer > pIndexBuffer ) const override;
        void UnbindIndexBuffer() const override;
        void BindShader( std::shared_ptr< Shader > pShader ) const override;
        void UnbindShader() const override;
        void BindVertexArrayObject( std::shared_ptr< VertexArrayObject > pVertexArrayObject ) const override;
        void UnbindVertexArrayObject() const override;
        void BindFramebuffer( std::shared_ptr< Framebuffer > pFramebuffer ) const override;
        void UnbindFramebuffer() const override;

        void UploadMat4( std::shared_ptr< Shader > pShader,
            const std::string &uniformName,
            const glm::mat4 &value ) const override;
        void UploadVec4( std::shared_ptr< Shader > pShader,
            const std::string &uniformName,
            const glm::vec4 &value ) const override;

        void FillVertexBuffer( std::shared_ptr< VertexBuffer > pVertexBuffer,
            const void *pData,
            Uint32 vertexCount ) const override;
        int ReadFramebufferPixel( std::shared_ptr< Framebuffer > pFramebuffer,
            Uint32 attachmentIndex,
            int x,
            int y ) const override;
        std::vector< int > ReadFramebufferPixels( std::shared_ptr< Framebuffer > pFramebuffer,
            Uint32 attachmentIndex,
            int x,
            int y,
            int width,
            int height ) const override;
        void ClearFramebufferAttachment( std::shared_ptr< Framebuffer > pFramebuffer,
            Uint32 attachmentIndex,
            int value ) const override;
    };
}