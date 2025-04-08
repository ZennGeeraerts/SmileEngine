/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    struct Texture final : public memory::Counted
    {
        Texture( TextureHandle handle ) : Handle{ handle }
        {
        }

        ~Texture() = default;
        Texture( const Texture & ) = delete;
        Texture( Texture && ) = delete;

        TextureHandle Handle;
    };
}
