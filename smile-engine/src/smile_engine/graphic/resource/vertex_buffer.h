#pragma once
#include "smile_engine/graphic/shader/buffer.h"

namespace smile::graphic
{
    struct VertexBufferDescriptor final
    {
        void *pVertices = nullptr;
        Uint32 Count = 0;
        Uint32 Stride = 0;
        BufferUsage Usage = BufferUsage::Default;
    };

    struct VertexBuffer
    {
        VertexBuffer() = default;
        virtual ~VertexBuffer() = default;
        virtual void *GetInternal() const = 0;

        Uint32 Stride = 0;
    };
}