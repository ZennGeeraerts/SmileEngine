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

        void BeginFrame();
        void EndFrame();

        void BindVertexBuffer( VertexBuffer::Ref pVertexBuffer ) const;
        void UnbindVertexBuffer() const;
        void FillVertexBuffer( VertexBuffer::Ref pVertexBuffer, void *pData, Count vertexCount ) const;

        void BindIndexBuffer( IndexBuffer::Ref pIndexBuffer ) const;
        void UnbindIndexBuffer() const;

        void BindConstantBuffer( ConstantBuffer::Ref pConstantBuffer ) const;
        void UnbindConstantBuffer() const;

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

        rhi::RendererBackendType GetRendererAPI() const
        {
            return m_API;
        }

        rhi::GraphicsDevice *GetGraphicsDevice() const
        {
            return m_pDevice.get();
        }

        rhi::CommandList *GetImmediateCommandList() const
        {
            return m_pImmediateCommandList;
        }

      private:
        rhi::RendererBackendType m_API;
        Scope< rhi::GraphicsDevice > m_pDevice;
        rhi::CommandList *m_pImmediateCommandList;
        ResourceManager m_ResourceManager{};

        math::Color m_ClearColor{};

        memory::Ref< rhi::SwapChain > m_pSwapChain = nullptr;
        memory::Ref< Shader > m_pBoundShader = nullptr;
        memory::Ref< Framebuffer > m_pBoundFramebuffer = nullptr;

        Index m_CurrentFrameIndex{ 0 };
        Index m_RenderedFrameIndex{ 0 };
    };
}
