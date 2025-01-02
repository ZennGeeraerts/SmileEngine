/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/engine/graphic/renderer_api/resource/frame_buffer.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Framebuffer final : public Framebuffer
    {
        DirectX11Framebuffer() = default;
        virtual ~DirectX11Framebuffer();

        DirectX11Framebuffer( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer( DirectX11Framebuffer && ) = delete;
        DirectX11Framebuffer &operator=( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer &operator=( DirectX11Framebuffer && ) = delete;

        virtual void *GetColor( Uint32 index ) const override
        {
            return pColorShaderResourceViews[index];
        }
        Uint32 GetRenderTargetViewCount() const override
        {
            return pRenderTargetViews.size();
        }
        void *GetRenderTargetViews() override
        {
            return static_cast< void * >( pRenderTargetViews.data() );
        }
        void *GetDepthStencilView() const override
        {
            return pDepthStencilView;
        }
        void *GetDepthStencilAttachment() const override
        {
            return pDepthStencilAttachment;
        }
        void *GetViewport() override
        {
            return &Viewport;
        }

        std::vector< ID3D11Texture2D * > pColorAttachments{};
        std::vector< ID3D11RenderTargetView * > pRenderTargetViews{};
        std::vector< ID3D11ShaderResourceView * > pColorShaderResourceViews{};

        ID3D11Texture2D *pDepthStencilAttachment = nullptr;
        ID3D11DepthStencilView *pDepthStencilView = nullptr;

        D3D11_VIEWPORT Viewport{};
    };
}
