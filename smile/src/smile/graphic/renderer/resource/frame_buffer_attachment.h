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
        FramebufferAttachment( Texture::Ref pTex, rhi::Format format, bool isReadOnly )
            : pTexture{ pTex }, TextureFormat{ format }, IsReadOnly{ isReadOnly }
        {
        }

        Texture::Ref pTexture;
        rhi::Format TextureFormat = rhi::Format::UNKNOWN;
        bool IsReadOnly = false;
    };
}