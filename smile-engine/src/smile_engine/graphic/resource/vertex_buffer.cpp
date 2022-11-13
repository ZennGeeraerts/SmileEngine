#include "smpch.h"
#include "vertex_buffer.h"

#include "smile_engine/graphic/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/resource/directx11_vertex_buffer.h"
#    include "platform/smileraster/smileraster_buffer.h"
#endif

namespace smile::graphic
{
    VertexBuffer *VertexBuffer::Create( const VertexBufferDescriptor &vertexBufferDesc )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "VertexBuffer::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11VertexBuffer{ vertexBufferDesc };

            case RendererAPI::API::SmileRaster:
                return new SmileRasterVertexBuffer{ vertexBufferDesc };
#endif
        }

        SM_ASSERT( false,
            "VertexBuffer::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}