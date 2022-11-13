#include "smpch.h"
#include "index_buffer.h"

#include "smile_engine/graphic/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/resource/directx11_index_buffer.h"
#    include "platform/smileraster/smileraster_buffer.h"
#endif

namespace smile::graphic
{
    IndexBuffer *IndexBuffer::Create( const IndexBufferDescriptor &indexBufferDesc )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "IndexBuffer::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11IndexBuffer( indexBufferDesc );

            case RendererAPI::API::SmileRaster:
                return new SmileRasterIndexBuffer( indexBufferDesc );
#endif
        }

        SM_ASSERT( false,
            "IndexBuffer::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}