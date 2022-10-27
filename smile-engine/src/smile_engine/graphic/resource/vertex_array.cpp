#include "smpch.h"
#include "vertex_array.h"

#include "smile_engine/graphic/renderer.h"

namespace smile::graphic
{
    VertexBuffer *VertexArray::create()
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "VertexArray::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return nullptr;

            case RendererAPI::API::SmileRaster:
                return nullptr;
#endif
        }

        SM_ASSERT( false,
            "VertexArray::create > return nullptr, unknown render api or render api is not supported on this "
            "platform" );
        return nullptr;
    }
}