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
        Uint32 VertexCount;
        Uint32 VertexOffset;
    };

    struct DrawIndexedParams final
    {
        Uint32 IndexCount;
        Uint32 IndexOffset;
        Uint32 VertexOffset;
    };
}