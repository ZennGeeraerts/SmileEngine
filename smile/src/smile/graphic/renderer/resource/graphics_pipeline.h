/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
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

        VertexShader::ConstRef pVertexShader;
        PixelShader::ConstRef pPixelShader;

        primitive::Vector< rhi::BindingLayout > BindingLayouts;
    };

    class GraphicsPipeline final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< GraphicsPipeline >;
        using ConstRef = memory::Ref< const GraphicsPipeline >;

        GraphicsPipeline( rhi::GraphicsPipelineHandle handle, const GraphicsPipelineDescriptor &desc )
            : m_Handle{ handle }, m_Descriptor{ desc }
        {
        }

        rhi::GraphicsPipelineHandle GetHandle() const
        {
            return m_Handle;
        }

        const GraphicsPipelineDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::GraphicsPipelineHandle m_Handle;
        GraphicsPipelineDescriptor m_Descriptor;

        friend class ResourceManager;
    };
}