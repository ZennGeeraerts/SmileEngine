#pragma once
#include "smile_engine/graphic/shader/buffer.h"

namespace smile::graphic
{
    struct IndexBufferDescriptor final
    {
        Uint32 *pIndices = nullptr;
        Uint32 Count = 0;
        BufferUsage Usage = BufferUsage::Default;
    };

    struct IndexBuffer
    {
        IndexBuffer() = default;
        virtual ~IndexBuffer() = default;
        virtual void *GetInternal() const = 0;

        Uint32 Count;
    };
}