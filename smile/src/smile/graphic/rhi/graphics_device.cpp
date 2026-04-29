/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "graphics_device.h"

#include "platform/d3d11/graphic/rhi/d3d11_device.h"

namespace smile::graphic::rhi
{
    memory::Scope< GraphicsDevice > GraphicsDevice::Create( RendererBackendType backendType )
    {
        switch ( backendType )
        {
            case RendererBackendType::D3D11:
                return memory::CreateScope< D3D11Device >();

            default:
                SM_ASSERT_MSG( false,
                    "GraphicsDevice::Create > unknown renderer backend or renderer backend is not supported on "
                    "this "
                    "platform" );
                break;
        }
    }
}