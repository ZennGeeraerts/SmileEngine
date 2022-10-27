#pragma once
#include "smile_engine/graphic/shader/buffer.h"

namespace smile::graphic
{
    struct IndexBufferDescriptor final
    {
        Uint32 *indices = nullptr;
        Uint32 count = 0;
        BufferUsage usage = BufferUsage::Default;
    };

    class IndexBuffer
    {
      public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual Uint32 getCount() const = 0;

        static IndexBuffer *create( const IndexBufferDescriptor &index_buffer_desc );
    };
}