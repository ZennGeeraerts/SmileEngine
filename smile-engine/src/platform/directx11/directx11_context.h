#pragma once
#include "smile_engine/graphic/graphics_context.h"

#include <d3d11.h>

namespace Smile
{
    class Window;
}

namespace Smile::Graphic
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

        virtual void Initialize() override;
        virtual void Present() override;

        inline ID3D11Device *GetDevice() const
        {
            return m_pDevice;
        }
        inline ID3D11DeviceContext *GetDeviceContext() const
        {
            return m_pDeviceContext;
        }
        inline ID3D11RenderTargetView *GetRenderTargetView() const
        {
            return m_pCurrentRenderTarget;
        }
        inline ID3D11DepthStencilView *GetDepthStencilView() const
        {
            return m_pDepthStencilView;
        }
        inline const D3D11_VIEWPORT &GetViewport() const
        {
            return m_Viewport;
        }

      private:
        Window *m_pWindow = nullptr;
        ID3D11Device *m_pDevice = nullptr;
        ID3D11DeviceContext *m_pDeviceContext = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;
        IDXGISwapChain *m_pSwapChain = nullptr;

        ID3D11RenderTargetView *m_pCurrentRenderTarget = nullptr;
        ID3D11DepthStencilView *m_pDepthStencilView = nullptr;

        ID3D11Texture2D *m_pDepthStencilBuffer = nullptr;
        ID3D11Resource *m_pRenderTargetBuffer = nullptr;

        D3D11_VIEWPORT m_Viewport;

        friend class DirectX11RendererAPI;
    };
}
