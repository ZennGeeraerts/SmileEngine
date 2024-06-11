/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_device.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    enum class PrimitiveTopology
    {
        None = 0,
        TriangleList,
        LineList
    };

    class GraphicsContext
    {
      public:
        virtual ~GraphicsContext() = default;
        virtual void Initialize( GraphicsDevice *pGraphicsDevice ) = 0;
        virtual void Present() = 0;

        virtual void *GetInternal() const = 0;

        virtual void BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const = 0;
        virtual void UnbindVertexBuffer() const = 0;
        virtual void BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const = 0;
        virtual void UnbindIndexBuffer() const = 0;
        virtual void BindShader( const Ref< Shader > &pShader ) const = 0;
        virtual void UnbindShader() const = 0;
        virtual void BindFramebuffer( const Ref< Framebuffer > &pFramebuffer ) const = 0;
        virtual void UnbindFramebuffer() const = 0;
        virtual void ClearFramebuffer( const Ref< Framebuffer > &pFramebuffer ) = 0;
        virtual void BindRasterizerState( const Ref< RasterizerState > &pRasterizerState ) const = 0;
        virtual void UnbindRasterizerState() const = 0;
        virtual void BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const = 0;
        virtual void UnbindPrimitiveTopology() const = 0;

        virtual void FillVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer, void *pData, Uint32 vertexCount ) const = 0;

        static GraphicsContext *Create( window::Window *pWindow );
    };
}