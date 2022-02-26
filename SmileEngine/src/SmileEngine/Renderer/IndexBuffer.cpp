#include "smpch.h"
#include "IndexBuffer.h"

#include "Renderer.h"
#ifdef SM_PLATFORM_WINDOWS
#    include "Platform/DirectX11/DirectX11IndexBuffer.h"
#endif

namespace smile
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

                /*case RendererAPI::API::SmileRaster:
                    return new SmileRasterIndexBuffer{ indexBufferData };*/
#endif
        }

        SM_ASSERT( false,
            "IndexBuffer::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}