#include "smpch.h"
#include "VertexBuffer.h"

#include "SmileEngine/Renderer/Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "Platform/DirectX11/Resource/DirectX11VertexBuffer.h"
#    include "Platform/SmileRaster/SmileRasterBuffer.h"
#endif

namespace smile
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
                return new DirectX11VertexBuffer( vertexBufferDesc );

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