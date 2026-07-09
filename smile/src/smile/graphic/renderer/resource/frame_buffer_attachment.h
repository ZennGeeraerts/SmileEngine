/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"
#include "texture.h"

namespace smile::graphic
{
    struct FramebufferAttachment final
    {
        FramebufferAttachment() = default;
        FramebufferAttachment( Texture tex, rhi::Format format, bool isReadOnly )
            : Texture{ tex }, TextureFormat{ format }, IsReadOnly{ isReadOnly }
        {
        }

        Texture Texture;
        rhi::Format TextureFormat = rhi::Format::UNKNOWN;
        bool IsReadOnly = false;
    };

    struct FramebufferAttachmentSet final
    {
        primitive::FixedVector< FramebufferAttachment, rhi::s_MaxRenderTargets > ColorAttachments{};
        FramebufferAttachment DepthAttachment{};

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash{ 0 };

            for ( const auto &attachment : ColorAttachments )
            {
                hash = foundation::HashCombine( hash, attachment.Texture.GetHandle().Hash() );

                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( attachment.TextureFormat ) ) );

                hash = foundation::HashCombine( hash, std::hash< bool >{}( attachment.IsReadOnly ) );
            }

            hash = foundation::HashCombine( hash, DepthAttachment.Texture.GetHandle().Hash() );

            hash = foundation::HashCombine(
                hash, std::hash< Uint8 >{}( static_cast< Uint8 >( DepthAttachment.TextureFormat ) ) );

            hash = foundation::HashCombine( hash, std::hash< bool >{}( DepthAttachment.IsReadOnly ) );

            return hash;
        }
    };

    using FramebufferAttachmentSetHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using FramebufferAttachmentSetHandle = FramebufferAttachmentSetHandleManager::HandleType;
}

namespace std
{
    template <>
    struct hash< smile::graphic::FramebufferAttachmentSet >
    {
        size_t operator()( const smile::graphic::FramebufferAttachmentSet &attachmentSet ) const noexcept
        {
            return attachmentSet.GetHashCode();
        }
    };
}