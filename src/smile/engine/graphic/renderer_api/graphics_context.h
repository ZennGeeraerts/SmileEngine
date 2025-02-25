/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "graphics_device.h"
#include "primitive_topology.h"
#include "memory/ref.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class GraphicsContext
    {
      public:
        virtual ~GraphicsContext() = default;

        virtual void *GetInternal() const = 0;

        virtual void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader ) = 0;
        virtual void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader ) = 0;
        virtual void Clear( const DirectX::XMFLOAT4 &clearColor ) = 0;

        virtual void BindVertexBuffer( const memory::Ref< VertexBuffer > &pVertexBuffer ) const = 0;
        virtual void UnbindVertexBuffer() const = 0;
        virtual void BindIndexBuffer( const memory::Ref< IndexBuffer > &pIndexBuffer ) const = 0;
        virtual void UnbindIndexBuffer() const = 0;
        virtual void BindShader( const memory::Ref< Shader > &pShader ) const = 0;
        virtual void UnbindShader() const = 0;
        virtual void BindFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) const = 0;
        virtual void UnbindFramebuffer() const = 0;
        virtual void ClearFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) = 0;
        virtual void BindRasterizerState( const memory::Ref< RasterizerState > &pRasterizerState ) const = 0;
        virtual void UnbindRasterizerState() const = 0;
        virtual void BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const = 0;
        virtual void UnbindPrimitiveTopology() const = 0;

        virtual void
        FillVertexBuffer( const memory::Ref< VertexBuffer > &pVertexBuffer, void *pData, Uint32 vertexCount ) const = 0;
    };
}