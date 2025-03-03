/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_backend.h"

#include "platform/directx11/graphic/renderer_backend/directx11_renderer_backend.h"

#if WITH_SMILERASTER
#    include "platform/smileraster/graphic/smileraster_renderer_api.h"
#endif

namespace smile::graphic
{
    Scope< RendererBackend > RendererBackend::Create( RendererBackend::API api )
    {
        switch ( api )
        {
            case RendererBackend::API::None:
                SM_ASSERT( false, "RendererBackend::Create > return nullptr, no renderer api selected" );
                break;

#ifdef SM_PLATFORM_WINDOWS
            case RendererBackend::API::DirectX11:
                return CreateScope< DirectX11RendererBackend >();

#    ifdef WITH_SMILERASTER
            case RendererAPI::API::SmileRaster:
                return CreateScope< SmileRasterRendererAPI >();
#    endif
#endif
            default:
                SM_ASSERT( false,
                    "RendererAPI::Create > return nullptr, unknown renderer api or renderer api is not supported on "
                    "this "
                    "platform" );
                break;
        }
    }
}