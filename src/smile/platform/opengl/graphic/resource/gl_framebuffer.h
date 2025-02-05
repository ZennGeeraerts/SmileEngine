/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/resource/frame_buffer.h"

namespace smile::graphic
{
    struct GLFramebuffer final : public Framebuffer
    {
        ~GLFramebuffer();

        void *GetInternal() const override
        {
            return reinterpret_cast< void * >( Internal );
        }

        void *GetColor( Uint32 index ) const override;

        Uint32 Internal;
        std::vector< Uint32 > ColorAttachments;
        Uint32 DepthAttachment;
    };
}