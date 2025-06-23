/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::graphic
{
    enum class FramebufferTextureFormat
    {
        None = 0,
        RGBA8,
        Depth24Stencil8,

        Depth = Depth24Stencil8
    };

    struct FramebufferTextureData final
    {
        FramebufferTextureData() = default;
        FramebufferTextureData( FramebufferTextureFormat format, bool useInShader = false )
            : TextureFormat{ format }, UseInShader{ useInShader }
        {
        }

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        bool UseInShader;
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentData final
    {
        FramebufferAttachmentData() = default;
        FramebufferAttachmentData( const std::initializer_list< FramebufferTextureData > &attachments )
            : Attachments{ attachments }
        {
        }

        std::vector< FramebufferTextureData > Attachments;
    };

    struct FramebufferDescriptor final
    {
        Uint32 Width = 0;
        Uint32 Height = 0;
        FramebufferAttachmentData Attachments;
        Uint16 Samples = 1;

        // TODO: if true -> Render to the swapchain
        bool IsSwapChainTarget = false;

        DirectX::XMFLOAT4 ClearColor = { 1.f, 1.f, 1.f, 1.f };
    };
}
