/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"

#include <wrl/client.h>

class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

namespace smile::graphic::rhi
{
    class D3D11Device;

    class D3D11Framebuffer final
    {
      public:
        D3D11Framebuffer() = default;
        ~D3D11Framebuffer();

        D3D11Framebuffer( const D3D11Framebuffer & ) = delete;
        D3D11Framebuffer( D3D11Framebuffer && ) = delete;
        D3D11Framebuffer &operator=( const D3D11Framebuffer & ) = delete;
        D3D11Framebuffer &operator=( D3D11Framebuffer && ) = delete;

        void Create( D3D11Device &device, const FramebufferDescriptor &desc );
        void Destroy();

        FramebufferDescriptor Descriptor{};
        FramebufferInfoExtented FramebufferInfo{};

        primitive::FixedVector< Microsoft::WRL::ComPtr< ID3D11RenderTargetView >, s_MaxRenderTargets >
            pRenderTargetViews{};
        Microsoft::WRL::ComPtr< ID3D11DepthStencilView > pDepthStencilView = nullptr;
    };
}
