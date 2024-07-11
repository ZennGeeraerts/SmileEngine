/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_command.h"

#include "platform/directx11/graphic/directx11_renderer_api.h"

#if WITH_SMILERASTER
#    include "platform/smileraster/graphic/smileraster_renderer_api.h"
#endif

namespace smile::graphic
{
    RendererAPI *RenderCommand::s_pRendererAPI = nullptr;

    void RenderCommand::Create( RendererAPI::API api )
    {
        switch ( api )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "RenderCommand::Create > return nullptr, no renderer api selected" );
                break;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                s_pRendererAPI = new DirectX11RendererAPI{};
                break;

#    ifdef WITH_SMILERASTER
            case RendererAPI::API::SmileRaster:
                s_pRendererAPI = new SmileRasterDevice{};
                break;
#    endif
#endif
            default:
                SM_ASSERT( false,
                    "RenderCommand::Create > return nullptr, unknown renderer api or renderer api is not supported on "
                    "this "
                    "platform" );
                break;
        }
    }
}