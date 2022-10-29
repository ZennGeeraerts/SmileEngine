#include "smpch.h"
#include "vertex_array.h"

#include "smile_engine/graphic/renderer.h"

namespace Smile::Graphic
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