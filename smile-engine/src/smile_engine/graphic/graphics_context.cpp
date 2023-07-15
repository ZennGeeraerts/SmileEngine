/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphics_context.h"

#include "render_engine.h"
#include "smile_engine/core/application.h"

#include "platform/directX11/directx11_context.h"

#ifdef WITH_SMILERASTER
    #include "platform/smileraster/smileraster_context.h"
#endif

namespace smile::graphic
{
    GraphicsContext *GraphicsContext::Create( Window *pWindow )
    {
        switch ( RenderEngine::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "GraphicsContext::Create > return nullptr, no renderer api selected" );
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
            "GraphicsContext::Create > return nullptr, unknown renderer api or renderer api is not supported on this "
            "platform" );
        return nullptr;
    }
}