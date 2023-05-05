#include "smpch.h"
#include "graphics_device.h"

#include "render_engine.h"
#include "smile_engine/core/application.h"

#include "platform/directx11/directx11_device.h"
#include "platform/smileraster/smile_raster_device.h"

namespace smile::graphic
{
    GraphicsDevice *GraphicsDevice::Create()
    {
        switch ( RenderEngine::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "GraphicsDevice::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return new DirectX11Device{};

#    ifdef WITH_SMILERASTER
            case RendererAPI::API::SmileRaster:
                return new SmileRasterDevice{};
#    endif
#endif
        }

        SM_ASSERT( false,
            "GraphicsDevice::Create > return nullptr, unknown renderer api or renderer api is not supported on this "
            "platform" );
        return nullptr;
    }
}