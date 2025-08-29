/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"

#include "resource/resource_manager.h"
#include "smile/graphic/rhi/graphics_device.h"
#include "smile/graphic/rhi/render_state.h"
#include "smile/graphic/rhi/swap_chain.h"

#include <DirectXMath.h>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class RenderSystem final
    {
      public:
        RenderSystem();
        ~RenderSystem();

        void Initialize( const window::Window *pWindow );

        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height );

        void SetClearColor( const DirectX::XMFLOAT4 &color )
        {
            m_ClearColor = color;
        }

        void Clear();

        void BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const;
        void UnbindVertexBuffer() const;
        void FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const;

        void BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const;
        void UnbindIndexBuffer() const;

        void BindUniformBuffer( const memory::Ref< UniformBuffer > &pUniformBuffer ) const;
        void UnbindUniformBuffer() const;

        void BindShader( memory::Ref< Shader > pShader );
        void UnbindShader();

        void BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer );
        void BindBackBuffer();

        void SetState( const RenderState &state ) const;

        void *ReadTexture( memory::Ref< Texture > pTexture ) const;
        void *ReadTexture( memory::Ref< Framebuffer > pFramebuffer, Uint32 index ) const;

        void DrawIndexed( Uint32 indexCount );
        void Draw( Uint32 vertexCount );
        void Present();

        ResourceManager &GetResourceManager()
        {
            return m_ResourceManager;
        }

        RendererBackendType GetRendererAPI() const
        {
            return m_API;
        }

        GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice.get();
        }

        CommandList *GetImmediateCommandList() const
        {
            return m_pImmediateCommandList;
        }

      private:
        RendererBackendType m_API;
        Scope< GraphicsDevice > m_pDevice;
        CommandList *m_pImmediateCommandList;
        ResourceManager m_ResourceManager{};

        DirectX::XMFLOAT4 m_ClearColor{};

        memory::Ref< SwapChain > m_pSwapChain = nullptr;
        memory::Ref< Shader > m_pBoundShader = nullptr;
        memory::Ref< Framebuffer > m_pBoundFramebuffer = nullptr;
    };
}
