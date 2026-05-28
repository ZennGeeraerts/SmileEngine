/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "skeleton.h"

namespace smile::graphic
{
    struct SkinnedMesh final
    {
        SkinnedMesh() = default;

        SkinnedMesh( const VertexBuffer &vb, const IndexBuffer &ib, Skeleton::Ref skeleton ) noexcept
            : VertexBuffer{ vb }, IndexBuffer{ ib }, Skeleton{ skeleton }
        {
        }

        [[nodiscard]] bool IsValid() const
        {
            return VertexBuffer.IsValid() && IndexBuffer.IsValid();
        }

        VertexBuffer VertexBuffer;
        IndexBuffer IndexBuffer;
        Skeleton::Ref Skeleton = nullptr;
    };
}