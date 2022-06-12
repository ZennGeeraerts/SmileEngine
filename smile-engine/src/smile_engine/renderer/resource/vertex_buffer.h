#pragma once
#include "smile_engine/renderer/shader/buffer.h"

namespace smile::renderer
{
    struct VertexBufferDescriptor final
    {
        void *vertices = nullptr;
        Uint32 count = 0;
        Uint32 stride = 0;
        BufferUsage usage = BufferUsage::Default;
    };

    class VertexBuffer
    {
      public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual Uint32 getStride() const = 0;

        static VertexBuffer *create( const VertexBufferDescriptor &vertex_buffer_desc );
    };
}