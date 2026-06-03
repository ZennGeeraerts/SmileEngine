/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::graphic::rhi
{
    struct DrawParams final
    {
        Count VertexCount;
        Uint32 VertexOffset;
    };

    struct DrawIndexedParams final
    {
        Count IndexCount;
        Uint32 IndexOffset;
        Uint32 VertexOffset;
    };

    struct DrawInstancedParams final
    {
        Count VertexCount;
        Count InstanceCount;
        Uint32 VertexOffset;
        Uint32 IndexOffset;
    };
}