#pragma once
#include "smile_engine/renderer/resource/vertex_buffer.h"
#include "smile_engine/renderer/resource/index_buffer.h"
#include "smileraster_context.h"

namespace smile::renderer
{
    class SmileRasterVertexBuffer final : public VertexBuffer
    {
      public:
        SmileRasterVertexBuffer( const VertexBufferDescriptor &vertex_buffer_descriptor );
        virtual ~SmileRasterVertexBuffer() = default;

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual Uint32 getStride() const
        {
            return stride;
        }

      private:
        SmileRasterContext *smileRasterContext = nullptr;
        Raster::BufferID vertexBuffer = SMR_INVALID_BUFFER_ID;
        Uint32 stride;
    };

    class SmileRasterIndexBuffer final : public IndexBuffer
    {
      public:
        SmileRasterIndexBuffer( const IndexBufferDescriptor &index_buffer_descriptor );
        virtual ~SmileRasterIndexBuffer() = default;

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual Uint32 getCount() const override
        {
            return count;
        }

      private:
        SmileRasterContext *smileRasterContext = nullptr;
        Raster::BufferID indexBuffer = SMR_INVALID_BUFFER_ID;
        Uint32 count;
    };
}
