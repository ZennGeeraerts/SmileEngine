/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/math/color.h"
#include "resource/buffer.h"
#include "resource/frame_buffer.h"
#include "resource/graphics_pipeline.h"
#include "shader/binding_set.h"
#include "viewport_state.h"

namespace smile::graphic::rhi
{
    struct GraphicsState final
    {
        GraphicsPipelineHandle Pipeline;
        FramebufferHandle Framebuffer;
        ViewportState Viewport;
        math::Color BlendConstantColor{};

        BindingSetVector Bindings;

        primitive::FixedVector< VertexBufferBinding, s_MaxVertexAttributeCount > VertexBuffers;
        IndexBufferBinding IndexBuffer;
    };
}