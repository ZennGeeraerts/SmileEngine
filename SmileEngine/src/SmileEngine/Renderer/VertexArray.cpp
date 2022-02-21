#include "smpch.h"
#include "VertexArray.h"

#include "Renderer.h"

namespace smile
{
    VertexBuffer *VertexArray::Create()
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "VertexArray::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return nullptr;

            case RendererAPI::API::SmileRaster:
                return nullptr;
#endif
        }

        SM_ASSERT( false,
            "VertexArray::Create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}