/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/resource/vertex_buffer.h"

#include <device_context.cuh>

namespace smile::graphic
{
    struct SmileRasterVertexBuffer final : public VertexBuffer
    {
        void *GetInternal() const override
        {
            return reinterpret_cast< void * >( Internal );
        }

        Raster::BufferID Internal = SMR_INVALID_BUFFER_ID;
    };
}