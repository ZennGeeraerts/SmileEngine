#pragma once
#include "smile_engine/graphic/resource/index_buffer.h"
#include "platform/directX11/directx11_context.h"

namespace Smile::Graphic
{
    class DirectX11IndexBuffer final : public IndexBuffer
    {
      public:
        DirectX11IndexBuffer( const IndexBufferDescriptor &indexBufferDesc );
        virtual ~DirectX11IndexBuffer();

        DirectX11IndexBuffer( const DirectX11IndexBuffer & ) = delete;
        DirectX11IndexBuffer( DirectX11IndexBuffer && ) = delete;
        DirectX11IndexBuffer &operator=( const DirectX11IndexBuffer & ) = delete;
        DirectX11IndexBuffer &operator=( DirectX11IndexBuffer && ) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual Uint32 GetCount() const override
        {
            return m_Count;
        }

      private:
        DirectX11Context *m_pDirectX11Context;
        ID3D11Buffer *m_pIndexBuffer;
        Uint32 m_Count;
    };
}
