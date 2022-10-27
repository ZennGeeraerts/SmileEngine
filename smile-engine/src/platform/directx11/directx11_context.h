#pragma once
#include "smile_engine/graphic/graphics_context.h"

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
        DirectX11Context( Window *window );
        virtual ~DirectX11Context();

        DirectX11Context( const DirectX11Context & ) = delete;
        DirectX11Context( DirectX11Context && ) = delete;
        DirectX11Context &operator=( const DirectX11Context & ) = delete;
        DirectX11Context &operator=( DirectX11Context && ) = delete;

        virtual void init() override;
        virtual void present() override;

        inline ID3D11Device *getDevice() const
        {
            return device;
        }
        inline ID3D11DeviceContext *getDeviceContext() const
        {
            return deviceContext;
        }
        inline ID3D11RenderTargetView *getRenderTargetView() const
        {
            return currentRenderTarget;
        }
        inline ID3D11DepthStencilView *getDepthStencilView() const
        {
            return depthStencilView;
        }
        inline const D3D11_VIEWPORT &getViewport() const
        {
            return viewport;
        }

      private:
        Window *window = nullptr;
        ID3D11Device *device = nullptr;
        ID3D11DeviceContext *deviceContext = nullptr;
        IDXGIFactory *dxgiFactory = nullptr;
        IDXGISwapChain *swapChain = nullptr;

        ID3D11RenderTargetView *currentRenderTarget = nullptr;
        ID3D11DepthStencilView *depthStencilView = nullptr;

        ID3D11Texture2D *depthStencilBuffer = nullptr;
        ID3D11Resource *renderTargetBuffer = nullptr;

        D3D11_VIEWPORT viewport;

        friend class DirectX11RendererAPI;
    };
}
