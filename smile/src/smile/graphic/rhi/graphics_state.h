/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "rhi.h"
#include "render_handle.h"
#include "resource/buffer.h"
#include "viewport_state.h"

namespace smile::graphic::rhi
{
    struct GraphicsState final
    {
        GraphicsPipelineHandle Pipeline;
        FramebufferHandle Framebuffer;
        ViewportState Viewport;
        BindingSetVector Bindings;

        primitive::FixedVector< VertexBufferBinding, s_MaxVertexAttributeCount > VertexBuffers;
        IndexBufferBinding IndexBuffer;
    };
}