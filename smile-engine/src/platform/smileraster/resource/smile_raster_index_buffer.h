#pragma once

#include "smile_engine/graphic/resource/index_buffer.h"

namespace smile::graphic
{
	struct SmileRasterIndexBuffer : public IndexBuffer
	{
        void *GetInternal() const override
        {
            return reinterpret_cast< void * >( Internal );
        }

        Raster::BufferID Internal = SMR_INVALID_BUFFER_ID;
	};
}