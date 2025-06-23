/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphics_device.h"

#include "platform/directx11/graphic/rhi/directx11_device.h"

namespace smile::graphic
{
    Scope< GraphicsDevice > GraphicsDevice::Create( RendererBackendType backendType )
    {
        switch ( backendType )
        {
            case RendererBackendType::DirectX11:
                return CreateScope< DirectX11Device >();

            default:
                SM_ASSERT_MSG( false,
                    "GraphicsDevice::Create > unknown renderer backend or renderer backend is not supported on "
                    "this "
                    "platform" );
                break;
        }
    }
}