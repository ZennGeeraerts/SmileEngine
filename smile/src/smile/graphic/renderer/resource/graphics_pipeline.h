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

        GraphicsPipeline( rhi::GraphicsPipelineHandle handle ) noexcept
            : m_Handle{ handle }
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