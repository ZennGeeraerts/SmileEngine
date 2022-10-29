#pragma once
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "platform/directX11/directx11_context.h"

namespace Smile::Graphic
{
    class DirectX11VertexBuffer final : public VertexBuffer
    {
      public:
        DirectX11VertexBuffer( const VertexBufferDescriptor &vertexBufferDesc );
        virtual ~DirectX11VertexBuffer();

        DirectX11VertexBuffer( const DirectX11VertexBuffer & ) = delete;
        DirectX11VertexBuffer( DirectX11VertexBuffer && ) = delete;
        DirectX11VertexBuffer &operator=( const DirectX11VertexBuffer & ) = delete;
        DirectX11VertexBuffer &operator=( DirectX11VertexBuffer && ) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual Uint32 GetStride() const override
        {
            return m_Stride;
        }

      private:
        DirectX11Context *m_pDirectX11Context;
        ID3D11Buffer *m_pVertexBuffer;
        Uint32 m_Stride = 0;
    };
}