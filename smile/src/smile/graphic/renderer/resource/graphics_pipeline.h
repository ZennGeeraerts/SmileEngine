/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/primitive_topology.h"
#include "smile/graphic/rhi/resource/buffer.h"
#include "smile/graphic/rhi/render_state.h"
#include "smile/graphic/rhi/shader/binding_layout.h"
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

        primitive::Vector< rhi::BindingLayout > BindingLayouts;
    };

    class GraphicsPipeline final
    {
      public:
        GraphicsPipeline() = default;

        GraphicsPipeline( rhi::GraphicsPipelineHandle handle, const GraphicsPipelineDescriptor &desc ) noexcept
            : m_Handle{ handle }, m_Descriptor{ desc }
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

        const GraphicsPipelineDescriptor &GetDescriptor() const noexcept
        {
            return m_Descriptor;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::GraphicsPipelineHandle m_Handle;
        GraphicsPipelineDescriptor m_Descriptor;

        friend class ResourceManager;
    };
}