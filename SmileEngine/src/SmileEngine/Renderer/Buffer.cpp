#include "smpch.h"
#include "Buffer.h"

#include "Renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "Platform/DirectX11/DirectX11Buffer.h"
#    include "Platform/SmileRaster/SmileRasterBuffer.h"
#endif

namespace smile
{
    VertexBuffer *VertexBuffer::Create( const VertexBufferData &vertexBufferData )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "VertexBuffer::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11VertexBuffer( vertexBufferData );

            case RendererAPI::API::SmileRaster:
                return new SmileRasterVertexBuffer{ vertexBufferData };
#endif
        }

        SM_ASSERT( false,
            "VertexBuffer::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }

    IndexBuffer *IndexBuffer::Create( const IndexBufferData &indexBufferData )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "IndexBuffer::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11IndexBuffer( indexBufferData );

            case RendererAPI::API::SmileRaster:
                return new SmileRasterIndexBuffer{ indexBufferData };
#endif
        }

        SM_ASSERT( false,
            "IndexBuffer::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}