#pragma once
#include "smile_engine/renderer/shader/buffer.h"

namespace smile
{
    struct IndexBufferDescriptor final
    {
        Uint32 *m_pIndices = nullptr;
        Uint32 m_Count = 0;
        BufferUsage m_Usage = BufferUsage::Default;
    };

    class IndexBuffer
    {
      public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;

        static IndexBuffer *Create( const IndexBufferDescriptor &indexBufferDesc );
    };
}