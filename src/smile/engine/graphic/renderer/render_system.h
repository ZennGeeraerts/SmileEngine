/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "foundation/pimpl.h"
#include "memory/ref.h"

#include "engine/graphic/renderer_api/primitive_topology.h"

#include <DirectXMath.h>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class RendererAPI;
    class ResourceManager;
    class VertexBuffer;
    class IndexBuffer;
    class Shader;
    class Framebuffer;
    class RasterizerState;

    class RenderSystem final
    {
      private:
        struct Opaque;

      public:
        RenderSystem();
        ~RenderSystem();

        void Initialize( window::Window *pWindow );

        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height );

        void SetClearColor( const DirectX::XMFLOAT4 &color );
        void Clear();

        void BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const;
        void UnbindVertexBuffer() const;
        void FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const;

        void BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const;
        void UnbindIndexBuffer() const;

        void BindShader( memory::Ref< Shader > pShader );
        void UnbindShader();

        void BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer ) const;
        void UnbindFramebuffer() const;
        void ClearFramebuffer( memory::Ref< Framebuffer > pFramebuffer );

        void BindRasterizerState( memory::Ref< RasterizerState > pRasterizerState ) const;
        void UnbindRasterizerState() const;

        void BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const;
        void UnbindPrimitiveTopology() const;

        void DrawIndexed( Uint32 indexCount );
        void Draw( Uint32 vertexCount );
        void Present();

        ResourceManager &GetResourceManager();

        RendererAPI *GetRendererAPI() const; // TODO: Remove

      private:
        foundation::PImpl< Opaque > m_pImpl;
    };
}
