/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_handle.h"
#include "resource/buffer.h"

namespace smile::graphic
{
    struct GraphicsState final
    {
        GraphicsPipelineHandle Pipeline;
        FramebufferHandle Framebuffer;
        std::vector< BindingSetHandle > Bindings;

        std::vector< VertexBufferBinding > VertexBuffers;
        IndexBufferBinding IndexBuffer;
    };
}