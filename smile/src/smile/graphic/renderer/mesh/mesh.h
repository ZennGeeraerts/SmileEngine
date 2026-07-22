/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "smile/graphic/rhi/primitive_topology.h"

namespace smile::graphic
{
    struct Mesh final
    {
        Mesh() = default;

        Mesh( const VertexBuffer &vb, const IndexBuffer &ib, const rhi::BufferLayout &vertexLayout ) noexcept
            : VertexBuffer{ vb }, IndexBuffer{ ib }, VertexLayout{ vertexLayout }
        {
        }

        Mesh( const VertexBuffer &vb,
            const IndexBuffer &ib,
            rhi::PrimitiveTopology topology,
            const rhi::BufferLayout &vertexLayout ) noexcept
            : VertexBuffer{ vb }, IndexBuffer{ ib }, Topology{ topology }, VertexLayout{ vertexLayout }
        {
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return VertexBuffer.IsValid() && IndexBuffer.IsValid();
        }

        VertexBuffer VertexBuffer;
        IndexBuffer IndexBuffer;
        rhi::PrimitiveTopology Topology = rhi::PrimitiveTopology::TriangleList;
        rhi::BufferLayout VertexLayout;
    };
}