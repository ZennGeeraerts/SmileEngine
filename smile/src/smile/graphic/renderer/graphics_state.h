/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/fixed_vector.h"

#include "resource/graphics_pipeline.h"
#include "resource/frame_buffer.h"
#include "shader/binding_set.h"
#include "shader/vertex_buffer_binding.h"
#include "shader/index_buffer_binding.h"
#include "smile/graphic/rhi/rhi.h"

namespace smile::graphic
{
    struct GraphicsState final
    {
        GraphicsPipeline Pipeline;
        Framebuffer Framebuffer;

        primitive::FixedVector< BindingSet, rhi::s_MaxBindingLayoutCount > Bindings;

        primitive::FixedVector< VertexBufferBinding, rhi::s_MaxVertexAttributeCount > VertexBuffers;
        IndexBufferBinding IndexBuffer;
    };
}