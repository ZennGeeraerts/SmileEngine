/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "buffer.h"
#include "smile/graphic/renderer_backend/render_state.h"
#include "smile/graphic/renderer_backend/render_handle.h"
#include "smile/graphic/renderer_backend/shader/binding_layout.h"

namespace smile::graphic
{
    struct GraphicsPipelineDescriptor final
    {
        BufferLayout InputLayout;
        RenderState State;

        ShaderHandle VertexShaderHandle;
        ShaderHandle PixelShaderHandle;

        std::vector< BindingLayout > BindingLayouts;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, InputLayout.GetHashCode() );
            hash = foundation::HashCombine( hash, State.GetHashCode() );
            hash = foundation::HashCombine( hash, VertexShaderHandle.Hash() );
            hash = foundation::HashCombine( hash, PixelShaderHandle.Hash() );
            return hash;
        }

        bool operator()( const GraphicsPipelineDescriptor &lhs, const GraphicsPipelineDescriptor &rhs )
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
        }
    };
}