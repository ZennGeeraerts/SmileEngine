#pragma once
#include "smile_engine/renderer/resource/vertex_buffer.h"
#include "platform/directX11/directx11_context.h"

namespace smile::renderer
{
    class DirectX11VertexBuffer final : public VertexBuffer
    {
      public:
        DirectX11VertexBuffer( const VertexBufferDescriptor &vertex_buffer_desc );
        virtual ~DirectX11VertexBuffer();

        DirectX11VertexBuffer( const DirectX11VertexBuffer & ) = delete;
        DirectX11VertexBuffer( DirectX11VertexBuffer && ) = delete;
        DirectX11VertexBuffer &operator=( const DirectX11VertexBuffer & ) = delete;
        DirectX11VertexBuffer &operator=( DirectX11VertexBuffer && ) = delete;

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual Uint32 getStride() const override
        {
            return stride;
        }

      private:
        DirectX11Context *directX11Context;
        ID3D11Buffer *vertexBuffer;
        Uint32 stride = 0;
    };
}