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

    class VertexBuffer
    {
      public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual Uint32 GetStride() const = 0;

        static VertexBuffer *Create( const VertexBufferDescriptor &vertexBufferDesc );
    };
}