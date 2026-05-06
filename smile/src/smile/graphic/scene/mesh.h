/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"

namespace smile::graphic
{
    struct Mesh final
    {
        Mesh() = default;

        Mesh( const VertexBuffer &vb, const IndexBuffer &ib ) noexcept : VertexBuffer{ vb }, IndexBuffer{ ib }
        {
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return VertexBuffer.IsValid() && IndexBuffer.IsValid();
        }

        VertexBuffer VertexBuffer;
        IndexBuffer IndexBuffer;
    };
}