/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_staging_texture.h"

#include "platform/d3d11/graphic/rhi/d3d11_context.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    D3D11StagingTexture::~D3D11StagingTexture()
    {
        Destroy();
    }

    void D3D11StagingTexture::Create( D3D11Context &context, const TextureDescriptor &desc )
    {
        SM_ASSERT( desc.CPUAccess != CPUAccessMode::None );

        Texture.Create( context.pDevice, desc, {} );
        CPUAccess = desc.CPUAccess;
    }

    void D3D11StagingTexture::Destroy()
    {
        Texture.Destroy();
    }

    void *D3D11StagingTexture::Map( D3D11Context &context, const TextureSlice &slice, CPUAccessMode cpuAccess )
    {
        SM_ASSERT( Texture.pInternal );

        auto resolvedSlice = slice.Resolve( Texture.Descriptor );

        D3D11_MAP mapType;
        switch ( cpuAccess )
        {
            case CPUAccessMode::Read:
                SM_ASSERT( CPUAccess == CPUAccessMode::Read );
                mapType = D3D11_MAP_READ;
                break;

            case CPUAccessMode::Write:
                SM_ASSERT( CPUAccess == CPUAccessMode::Write );
                mapType = D3D11_MAP_WRITE;
                break;

            default:
                SM_ASSERT_MSG( false, "D3D11StagingTexture::Map > Unsupported CPUAccessMode" );
                return nullptr;
        }

        UINT subresource =
            D3D11CalcSubresource( resolvedSlice.MipLevelCount, resolvedSlice.Slice, Texture.Descriptor.MipLevelCount );

        D3D11_MAPPED_SUBRESOURCE resource;
        HRESULT result = context.pImmediateContext->Map( Texture.pInternal.Get(), subresource, mapType, 0, &resource );

        if ( SUCCEEDED( result ) )
        {
            MappedSubresource = subresource;
            return resource.pData;
        }
        else
        {
            return nullptr;
        }
    }

    void D3D11StagingTexture::Unmap( D3D11Context &context )
    {
        SM_ASSERT( MappedSubresource != std::numeric_limits< Uint32 >::max() );

        context.pImmediateContext->Unmap( Texture.pInternal.Get(), MappedSubresource );
        MappedSubresource = std::numeric_limits< Uint32 >::max();
    }
}