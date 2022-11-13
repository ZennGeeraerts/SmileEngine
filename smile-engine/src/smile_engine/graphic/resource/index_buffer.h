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

    class IndexBuffer
    {
      public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual Uint32 GetCount() const = 0;

        static IndexBuffer *Create( const IndexBufferDescriptor &indexBufferDesc );
    };
}