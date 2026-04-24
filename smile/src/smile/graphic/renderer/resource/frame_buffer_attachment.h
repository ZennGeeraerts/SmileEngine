/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/fixed_vector.h"
#include "texture.h"
#include "smile/graphic/rhi/format.h"
#include "smile/graphic/rhi/rhi.h"

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
    };

    using FramebufferAttachmentSetHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using FramebufferAttachmentSetHandle = FramebufferAttachmentSetHandleManager::HandleType;
}