/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/engine/graphic/resource/index_buffer.h"

#include <device_context.cuh>

namespace smile::graphic
{
	struct SmileRasterIndexBuffer final : public IndexBuffer
	{
        void *GetInternal() const override
        {
            return reinterpret_cast< void * >( Internal );
        }

        Raster::BufferID Internal = SMR_INVALID_BUFFER_ID;
	};
}