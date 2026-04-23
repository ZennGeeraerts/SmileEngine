/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "texture.h"
#include "smile/graphic/rhi/format.h"

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
}