#pragma once
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "smile_engine/graphic/resource/index_buffer.h"
#include "smileraster_context.h"

namespace smile::graphic
{
    class SmileRasterVertexBuffer final : public VertexBuffer
    {
      public:
        SmileRasterVertexBuffer( const VertexBufferDescriptor &vertexBufferDescriptor );
        virtual ~SmileRasterVertexBuffer() = default;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual Uint32 GetStride() const
        {
            return m_Stride;
        }

      private:
        SmileRasterContext *m_pSmileRasterContext = nullptr;
        Raster::BufferID m_VertexBuffer = SMR_INVALID_BUFFER_ID;
        Uint32 m_Stride;
    };

    class SmileRasterIndexBuffer final : public IndexBuffer
    {
      public:
        SmileRasterIndexBuffer( const IndexBufferDescriptor &indexBufferDescriptor );
        virtual ~SmileRasterIndexBuffer() = default;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual Uint32 GetCount() const override
        {
            return m_Count;
        }

      private:
        SmileRasterContext *m_pSmileRasterContext = nullptr;
        Raster::BufferID m_IndexBuffer = SMR_INVALID_BUFFER_ID;
        Uint32 m_Count;
    };
}
