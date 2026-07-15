/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "smile/graphic/rhi/primitive_topology.h"
#include "mesh_handle.h"

namespace smile::graphic
{
    struct Mesh final
    {
        Mesh() = default;

        Mesh( const MeshHandle &handle, const VertexBuffer &vb, const IndexBuffer &ib ) noexcept
            : Handle{ handle }, VertexBuffer{ vb }, IndexBuffer{ ib }
        {
        }

        Mesh( const MeshHandle &handle,
            const VertexBuffer &vb,
            const IndexBuffer &ib,
            rhi::PrimitiveTopology topology ) noexcept
            : Handle{ handle }, VertexBuffer{ vb }, IndexBuffer{ ib }, Topology{ topology }
        {
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return Handle.IsValid();
        }

        MeshHandle Handle;
        VertexBuffer VertexBuffer;
        IndexBuffer IndexBuffer;
        rhi::PrimitiveTopology Topology = rhi::PrimitiveTopology::TriangleList;
    };
}