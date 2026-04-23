/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "smile/graphic/rhi/format.h"

namespace smile::graphic
{
    struct IndexBufferBinding final
    {
        IndexBuffer IndexBuffer;
        rhi::Format BufferFormat;
        Uint32 Offset;
    };
}