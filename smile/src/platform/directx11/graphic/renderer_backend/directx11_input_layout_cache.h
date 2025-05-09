/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "smile/graphic/renderer_backend/resource/graphics_pipeline.h"

#include "directx11_state_cache.h"
#include "shader/directx11_input_layout.h"

namespace smile::graphic
{
    namespace detail
    {
        struct GraphicsPipelineDescriptorHasher final
        {
            foundation::HashCode operator()( const GraphicsPipelineDescriptor &pipelineDesc ) const
            {
                foundation::HashCode hash = 0;

                hash ^= std::hash< Uint32 >{}( pipelineDesc.InputLayout.GetStride() );

                for ( const auto &elem : pipelineDesc.InputLayout )
                {
                    hash ^= std::hash< std::string >{}( elem.Name );
                    hash ^= std::hash< Uint32 >{}( static_cast< Uint32 >( elem.FormatType ) );
                    hash ^= std::hash< Uint32 >{}( elem.Size );
                    hash ^= std::hash< Uint32 >{}( elem.Offset );
                }

                hash ^= std::hash< Uint64 >{}( static_cast< Uint64 >( pipelineDesc.VertexShaderHandle ) );

                return hash;
            }
        };

        struct GraphicsPipelineDescriptorComparer final
        {
            bool operator()( const GraphicsPipelineDescriptor &lhs, const GraphicsPipelineDescriptor &rhs ) const
            {
                return lhs.InputLayout == rhs.InputLayout && lhs.VertexShaderHandle == rhs.VertexShaderHandle;
            }
        };
    }

    using DirectX11InputLayoutCache = typename DirectX11StateCache< GraphicsPipelineDescriptor,
        DirectX11InputLayout,
        detail::GraphicsPipelineDescriptorHasher,
        detail::GraphicsPipelineDescriptorComparer >;

}