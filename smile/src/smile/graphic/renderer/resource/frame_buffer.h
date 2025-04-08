/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    struct Framebuffer final : public memory::Counted
    {
      public:
        Framebuffer( FramebufferHandle handle ) : Handle{ handle }
        {
        }
        ~Framebuffer() = default;

        FramebufferHandle Handle;
        Uint32 Width{};
        Uint32 Height{};

        static constexpr Uint32 MaxFramebufferSize{ 8192 };
    };
}