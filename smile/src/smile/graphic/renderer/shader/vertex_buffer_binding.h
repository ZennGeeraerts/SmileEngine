/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/graphic/renderer/resource/vertex_buffer.h"

namespace smile::graphic
{
    struct VertexBufferBinding final
    {
        VertexBuffer VertexBuffer;
        Uint32 Slot;
        Uint64 Offset;
    };
}