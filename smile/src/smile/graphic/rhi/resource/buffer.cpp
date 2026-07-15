/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "buffer.h"

namespace smile::graphic::rhi
{
    BufferRange BufferRange::Resolve( const GPUBufferDescriptor &bufferDesc ) const
    {
        BufferRange result;

        result.Offset = std::min( Offset, bufferDesc.Size );

        if ( Size == 0 )
            result.Size = bufferDesc.Size - result.Offset;
        else
            result.Size = std::min( Size, bufferDesc.Size - result.Offset );

        return result;
    }
}