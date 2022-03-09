#pragma once
#include "SmileEngine/Renderer/Shader/Buffer.h"

namespace smile
{
    struct VertexBufferDescriptor final
    {
        void *m_pVertices = nullptr;
        Uint32 m_Count = 0;
        Uint32 m_Stride = 0;
        BufferUsage m_Usage = BufferUsage::Default;
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