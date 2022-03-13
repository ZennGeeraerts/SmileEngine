#pragma once
#include "smile_engine/renderer/resource/index_buffer.h"
#include "platform/directX11/directx11_context.h"

namespace smile
{
    class DirectX11IndexBuffer final : public IndexBuffer
    {
      public:
        DirectX11IndexBuffer( const IndexBufferDescriptor &indexBufferDesc );
        virtual ~DirectX11IndexBuffer();

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
