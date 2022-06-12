#pragma once
#include "smile_engine/renderer/resource/index_buffer.h"
#include "platform/directX11/directx11_context.h"

namespace smile::renderer
{
    class DirectX11IndexBuffer final : public IndexBuffer
    {
      public:
        DirectX11IndexBuffer( const IndexBufferDescriptor &index_buffer_desc );
        virtual ~DirectX11IndexBuffer();

        DirectX11IndexBuffer( const DirectX11IndexBuffer & ) = delete;
        DirectX11IndexBuffer( DirectX11IndexBuffer && ) = delete;
        DirectX11IndexBuffer &operator=( const DirectX11IndexBuffer & ) = delete;
        DirectX11IndexBuffer &operator=( DirectX11IndexBuffer && ) = delete;

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual Uint32 getCount() const override
        {
            return count;
        }

      private:
        DirectX11Context *directX11Context;
        ID3D11Buffer *indexBuffer;
        Uint32 count;
    };
}
