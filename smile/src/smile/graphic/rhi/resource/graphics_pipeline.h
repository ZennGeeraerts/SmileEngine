/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "buffer.h"
#include "smile/graphic/rhi/primitive_topology.h"
#include "smile/graphic/rhi/render_state.h"
#include "smile/graphic/rhi/shader/shader.h"
#include "smile/graphic/rhi/shader/binding_layout.h"

namespace smile::graphic::rhi
{
    struct GraphicsPipelineDescriptor final
    {
        PrimitiveTopology Topology;
        BufferLayout InputLayout;
        RenderState State;

        ShaderHandle VertexShaderHandle;
        ShaderHandle PixelShaderHandle;

        BindingLayoutVector BindingLayouts;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = std::hash< Uint8 >{}( static_cast< Uint8 >( Topology ) );
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

    using GraphicsPipelineHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using GraphicsPipelineHandle = GraphicsPipelineHandleManager::HandleType;

    static constexpr Uint16 s_MaxGraphicsPipelineCount = ( 4 << 10 );
}