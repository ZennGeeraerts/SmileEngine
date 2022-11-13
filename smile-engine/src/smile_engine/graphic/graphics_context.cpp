#include "smpch.h"
#include "graphics_context.h"

#include "renderer.h"
#include "smile_engine/core/window.h"

#include "platform/directX11/directx11_context.h"
#include "platform/smileraster/smileraster_context.h"

namespace smile::graphic
{
    GraphicsContext *GraphicsContext::Create( Window *pWindow )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "RenderingContext::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11Context{ pWindow };
#    ifdef WITH_SMILERASTER
            case RendererAPI::API::SmileRaster:
                return new SmileRasterContext{ pWindow };
#    endif
#endif
        }

        SM_ASSERT( false,
            "RenderingContext::Create > return nullptr, unknown renderer api or renderer api is not supported on this "
            "platform" );
        return nullptr;
    }
}