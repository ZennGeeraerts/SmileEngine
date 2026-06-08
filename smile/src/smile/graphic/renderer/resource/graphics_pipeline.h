/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/graphic/rhi/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/shader/binding_layout.h"
#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"

namespace smile::graphic
{
    struct GraphicsPipelineDescriptor final
    {
        GraphicsPipelineDescriptor() = default;

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< rhi::PrimitiveTopology >{}( Topology );
            hash = foundation::HashCombine( hash, InputLayout.GetHashCode() );
            hash = foundation::HashCombine( hash, RenderState.GetHashCode() );
            hash = foundation::HashCombine( hash, VertexShader.GetHandle().Hash() );
            hash = foundation::HashCombine( hash, PixelShader.GetHandle().Hash() );

            for ( const auto &bindingLayout : BindingLayouts )
                hash = foundation::HashCombine( hash, bindingLayout.GetHandle().Hash() );

            return hash;
        }

        bool operator==( const GraphicsPipelineDescriptor &other ) const noexcept
        {
            return Topology == other.Topology && InputLayout == other.InputLayout && RenderState == other.RenderState &&
                   VertexShader == other.VertexShader && PixelShader == other.PixelShader &&
                   BindingLayouts == other.BindingLayouts;
        }

        bool operator!=( const GraphicsPipelineDescriptor &other ) const noexcept
        {
            return !( *this == other );
        }

        rhi::PrimitiveTopology Topology;
        rhi::BufferLayout InputLayout;
        rhi::RenderState RenderState;

        VertexShader VertexShader;
        PixelShader PixelShader;

        primitive::FixedVector< BindingLayout, rhi::s_MaxBindingLayoutCount > BindingLayouts;
    };

    class GraphicsPipeline final
    {
      public:
        GraphicsPipeline() = default;

        GraphicsPipeline( rhi::GraphicsPipelineHandle handle ) noexcept : m_Handle{ handle }
        {
        }

        GraphicsPipeline( const GraphicsPipeline & ) = default;
        GraphicsPipeline( GraphicsPipeline && ) noexcept = default;

        ~GraphicsPipeline() = default;

        GraphicsPipeline &operator=( const GraphicsPipeline & ) = default;
        GraphicsPipeline &operator=( GraphicsPipeline && ) noexcept = default;

        rhi::GraphicsPipelineHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        bool operator==( const GraphicsPipeline &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::GraphicsPipelineHandle m_Handle;

        friend class ResourceManager;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::GraphicsPipelineDescriptor >
    {
        smile::foundation::HashCode operator()( const smile::graphic::GraphicsPipelineDescriptor &desc ) const noexcept
        {
            return desc.GetHashCode();
        }
    };
}