#pragma once
#include "smile_engine/graphic/graphics_context.h"
#include "resource/directx11_frame_buffer.h"

#include <d3d11.h>

namespace smile
{
    class Window;
}

namespace smile::graphic
{
    class DirectX11Context final : public GraphicsContext
    {
      public:
        DirectX11Context( Window *pWindow );
        virtual ~DirectX11Context();

        DirectX11Context( const DirectX11Context & ) = delete;
        DirectX11Context( DirectX11Context && ) = delete;
        DirectX11Context &operator=( const DirectX11Context & ) = delete;
        DirectX11Context &operator=( DirectX11Context && ) = delete;

        virtual void Initialize( GraphicsDevice *pGraphicsDevice ) override;
        virtual void Present() override;

        void *GetInternal() const override
        {
            return m_pInternal;
        }

        void BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const override;
        void UnbindVertexBuffer() const override;
        void BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const override;
        void UnbindIndexBuffer() const override;
        void BindShader( const Ref< Shader > &pShader ) const override;
        void UnbindShader() const override;
        void BindFramebuffer( const Ref< Framebuffer > &pFramebuffer ) const override;
        void UnbindFramebuffer() const override;
        void ClearFramebuffer( const Ref< Framebuffer > &pFramebuffer ) override;
        void BindRasterizerState( const Ref< RasterizerState > &pRasterizerState ) const override;
        void UnbindRasterizerState() const override;

        inline ID3D11RenderTargetView *GetRenderTargetView() const
        {
            return m_pCurrentRenderTarget;
        }
        inline ID3D11DepthStencilView *GetDepthStencilView() const
        {
            return m_pSwapChainTarget->pDepthStencilView;
        }
        inline const D3D11_VIEWPORT &GetViewport() const
        {
            return m_Viewport;
        }

      private:
        Window *m_pWindow = nullptr;
        ID3D11DeviceContext *m_pInternal = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;
        IDXGISwapChain *m_pSwapChain = nullptr;

        Ref< DirectX11Framebuffer > m_pSwapChainTarget = nullptr;

        ID3D11RenderTargetView *m_pCurrentRenderTarget = nullptr;
        ID3D11Resource *m_pRenderTargetBuffer = nullptr;

        D3D11_VIEWPORT m_Viewport{};

        friend class DirectX11Device;
        friend class DirectX11RendererAPI;
    };
}
