#include "smpch.h"
#include "GraphicsContext.h"

#include "Renderer.h"
#include "SmileEngine/Core/Window.h"

#include "Platform/DirectX11/DirectX11Context.h"
#include "Platform/SmileRaster/SmileRasterContext.h"

namespace smile
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

            case RendererAPI::API::SmileRaster:
                return new SmileRasterContext{ pWindow };
#endif
        }

        SM_ASSERT( false,
            "RenderingContext::Create > return nullptr, unknown renderer api or renderer api is not supported on this "
            "platform" );
        return nullptr;
    }
}