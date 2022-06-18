#include "smpch.h"
#include "graphics_context.h"

#include "renderer.h"
#include "smile_engine/core/window.h"

#include "platform/directX11/directx11_context.h"
#include "platform/smileraster/smileraster_context.h"

namespace smile::renderer
{
    GraphicsContext *GraphicsContext::create( Window *window )
    {
        switch ( Renderer::getAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "RenderingContext::create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11Context{ window };
#    ifdef WITH_SMILERASTER
            case RendererAPI::API::SmileRaster:
                return new SmileRasterContext{ window };
#    endif
#endif
        }

        SM_ASSERT( false,
            "RenderingContext::create > return nullptr, unknown renderer api or renderer api is not supported on this "
            "platform" );
        return nullptr;
    }
}