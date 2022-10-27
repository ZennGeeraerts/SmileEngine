#include "smpch.h"
#include "index_buffer.h"

#include "smile_engine/graphic/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/resource/directx11_index_buffer.h"
#    include "platform/smileraster/smileraster_buffer.h"
#endif

namespace smile::graphic
{
    IndexBuffer *IndexBuffer::create( const IndexBufferDescriptor &index_buffer_desc )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "IndexBuffer::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11IndexBuffer( index_buffer_desc );

            case RendererAPI::API::SmileRaster:
                return new SmileRasterIndexBuffer( index_buffer_desc );
#endif
        }

        SM_ASSERT( false,
            "IndexBuffer::create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}