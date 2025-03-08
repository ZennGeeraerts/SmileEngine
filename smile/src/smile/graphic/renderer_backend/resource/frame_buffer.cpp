/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "frame_buffer.h"

namespace smile::graphic
{
    const Uint32 Framebuffer::MaxFramebufferSize = 8192;

    bool Framebuffer::IsDepthFormat( FramebufferTextureFormat format )
    {
        switch ( format )
        {
            case FramebufferTextureFormat::Depth24Stencil8:
                return true;

            default:
                return false;
        }
    }
}