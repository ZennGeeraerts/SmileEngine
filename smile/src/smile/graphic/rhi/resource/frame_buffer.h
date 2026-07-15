/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "texture.h"
#include "smile/graphic/rhi/viewport.h"
#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/common/primitive/collection/array_utils.h"

namespace smile::graphic::rhi
{
    static constexpr Uint32 s_MaxFramebufferSize = 8192;
    static constexpr Uint16 s_MaxFramebufferCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxRenderTargets = 8;

    struct FramebufferAttachment final
    {
        FramebufferAttachment() = default;
        FramebufferAttachment( TextureHandle handle, const TextureDescriptor &textureDesc )
            : Texture{ handle }, TextureDesc{ textureDesc }
        {
        }

        TextureHandle Texture;
        TextureDescriptor TextureDesc;
        TextureSubresourceSet Subresources{ 0, 1, 0, 1 };

        bool IsValid() const
        {
            return Texture.IsValid();
        }
    };

    struct FramebufferDescriptor final
    {
        primitive::FixedVector< FramebufferAttachment, s_MaxRenderTargets > ColorAttachments;
        FramebufferAttachment DepthAttachment;
    };

    struct FramebufferInfo
    {
        FramebufferInfo() = default;
        FramebufferInfo( const FramebufferDescriptor &desc );

        inline bool operator==( const FramebufferInfo &other ) const
        {
            return primitive::array::IsEqual( ColorFormats, other.ColorFormats ) && DepthFormat == other.DepthFormat &&
                   SampleCount == other.SampleCount && SampleQuality == other.SampleQuality;
        }

        inline bool operator!=( const FramebufferInfo &other ) const
        {
            return !( *this == other );
        }

        primitive::FixedVector< Format, s_MaxRenderTargets > ColorFormats;
        Format DepthFormat = Format::UNKNOWN;
        Count SampleCount = 1;
        Uint32 SampleQuality = 0;
    };

    struct FramebufferInfoExtented final : public FramebufferInfo
    {
        FramebufferInfoExtented() = default;
        FramebufferInfoExtented( const FramebufferDescriptor &desc );

        [[nodiscard]] Viewport GetViewport( const float minZ = 0.0f, const float maxZ = 0.0f ) const
        {
            return Viewport{ 0.0f, static_cast< float >( Width ), 0.0f, static_cast< float >( Height ), minZ, maxZ };
        }

        Uint32 Width{ 0 };
        Uint32 Height{ 0 };
    };

    using FramebufferHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using FramebufferHandle = FramebufferHandleManager::HandleType;
}
