/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphics_device.h"

#include "platform/directx11/graphic/renderer_backend/directx11_device.h"

namespace smile::graphic
{
    GraphicsDevice *GraphicsDevice::Create( RendererBackendType backendType )
    {
        switch ( backendType )
        {
            case RendererBackendType::DirectX11:
                return new DirectX11Device{};

            default:
                SM_ASSERT( false,
                    "GraphicsDevice::Create > unknown renderer backend or renderer backend is not supported on "
                    "this "
                    "platform" );
                break;
        }
    }
}