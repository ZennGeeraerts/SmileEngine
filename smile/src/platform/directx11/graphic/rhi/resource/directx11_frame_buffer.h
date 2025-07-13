/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"

#include <wrl/client.h>

namespace smile::graphic::rhi
{
    class DirectX11Device;

    class DirectX11Framebuffer final
    {
      public:
        DirectX11Framebuffer() = default;
        ~DirectX11Framebuffer() = default;

        DirectX11Framebuffer( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer( DirectX11Framebuffer && ) = delete;
        DirectX11Framebuffer &operator=( const DirectX11Framebuffer & ) = delete;
        DirectX11Framebuffer &operator=( DirectX11Framebuffer && ) = delete;

        void Create( DirectX11Device &device, const FramebufferDescriptor &desc );
        void Destroy();

        FramebufferDescriptor Descriptor{};
        FramebufferInfoExtented FramebufferInfo{};

        primitive::FixedVector< Microsoft::WRL::ComPtr< ID3D11RenderTargetView >, s_MaxRenderTargets >
            pRenderTargetViews{};
        Microsoft::WRL::ComPtr< ID3D11DepthStencilView > pDepthStencilView = nullptr;
    };
}
