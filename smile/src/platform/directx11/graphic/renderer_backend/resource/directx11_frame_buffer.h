/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/frame_buffer.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Framebuffer final
    {
        DirectX11Framebuffer() = default;
        ~DirectX11Framebuffer() = default;

        DirectX11Framebuffer( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer( DirectX11Framebuffer && ) = delete;
        DirectX11Framebuffer &operator=( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer &operator=( DirectX11Framebuffer && ) = delete;

        void Create( ID3D11Device *pDevice, const FramebufferDescriptor &desc );
        void Destroy();
        void Invalidate( ID3D11Device *pDevice );

        FramebufferDescriptor Descriptor{};
        std::vector< FramebufferTextureData > ColorAttachmentData{};
        FramebufferTextureData DepthAttachmentData = FramebufferTextureFormat::None;

        std::vector< ID3D11Texture2D * > pColorAttachments{};
        std::vector< ID3D11RenderTargetView * > pRenderTargetViews{};
        std::vector< ID3D11ShaderResourceView * > pColorShaderResourceViews{};

        ID3D11Texture2D *pDepthStencilAttachment = nullptr;
        ID3D11DepthStencilView *pDepthStencilView = nullptr;

        D3D11_VIEWPORT Viewport{};
    };
}
