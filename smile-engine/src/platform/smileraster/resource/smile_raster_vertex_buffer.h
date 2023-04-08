#pragma once

#include "smile_engine/graphic/resource/vertex_buffer.h"

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