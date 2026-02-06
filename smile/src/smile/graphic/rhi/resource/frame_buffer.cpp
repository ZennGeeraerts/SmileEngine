/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "frame_buffer.h"

#include "smile/common/foundation/range_iterator.h"

namespace smile::graphic::rhi
{
    FramebufferInfo::FramebufferInfo( const FramebufferDescriptor &desc )
    {
        for ( const auto &attachment : desc.ColorAttachments )
        {
            ColorFormats.PushBack( attachment.TextureDesc.TextureFormat );
        }

        if ( desc.DepthAttachment.IsValid() )
        {
            DepthFormat = desc.DepthAttachment.TextureDesc.TextureFormat;
            SampleCount = desc.DepthAttachment.TextureDesc.SampleCount;
            SampleQuality = desc.DepthAttachment.TextureDesc.SampleQuality;
        }
        else if ( !desc.ColorAttachments.IsEmpty() && desc.ColorAttachments[0].IsValid() )
        {
            const TextureDescriptor &textureDesc = desc.ColorAttachments[0].TextureDesc;
            SampleCount = textureDesc.SampleCount;
            SampleQuality = textureDesc.SampleQuality;
        }
    }

    FramebufferInfoExtented::FramebufferInfoExtented( const FramebufferDescriptor &desc ) : FramebufferInfo{ desc }
    {
        if ( desc.DepthAttachment.IsValid() )
        {
            Width = std::max(
                desc.DepthAttachment.TextureDesc.Width >> desc.DepthAttachment.Subresources.BaseMipLevel, 1u );
            Height = std::max(
                desc.DepthAttachment.TextureDesc.Height >> desc.DepthAttachment.Subresources.BaseMipLevel, 1u );
        }
        else if ( !desc.ColorAttachments.IsEmpty() && desc.ColorAttachments[0].IsValid() )
        {
            const TextureDescriptor &textureDesc = desc.ColorAttachments[0].TextureDesc;
            Width = std::max( textureDesc.Width >> desc.ColorAttachments[0].Subresources.BaseMipLevel, 1u );
            Height = std::max( textureDesc.Height >> desc.ColorAttachments[0].Subresources.BaseMipLevel, 1u );
        }
    }
}