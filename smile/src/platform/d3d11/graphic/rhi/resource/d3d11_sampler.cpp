/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_sampler.h"

namespace smile::graphic::rhi
{
    static D3D11_FILTER SamplerFilteringToD3D11Type( SamplerFiltering filtering )
    {
        switch ( filtering )
        {
            case SamplerFiltering::MinMagMipPoint:
                return D3D11_FILTER_MIN_MAG_MIP_POINT;
            case SamplerFiltering::MinMagMipLinear:
                return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            default:
                SM_ASSERT_MSG( false, "Unknown filter" );
        }
    }

    static D3D11_TEXTURE_ADDRESS_MODE SamplerAddressingToD3D11Type( SamplerAddressing addressing )
    {
        switch ( addressing )
        {
            case SamplerAddressing::Wrap:
                return D3D11_TEXTURE_ADDRESS_WRAP;
            case SamplerAddressing::Clamp:
                return D3D11_TEXTURE_ADDRESS_CLAMP;
            default:
                SM_ASSERT_MSG( false, "Unknown addressing" );
        }
    }

    D3D11Sampler::~D3D11Sampler()
    {
        Destroy();
    }

    void D3D11Sampler::Create( ID3D11Device *pDevice, const SamplerDescriptor &samplerDesc )
    {
        D3D11_SAMPLER_DESC desc{};
        desc.Filter = SamplerFilteringToD3D11Type( samplerDesc.Filtering );
        desc.AddressU = SamplerAddressingToD3D11Type( samplerDesc.AddressingU );
        desc.AddressV = SamplerAddressingToD3D11Type( samplerDesc.AddressingV );
        desc.AddressW = SamplerAddressingToD3D11Type( samplerDesc.AddressingW );

        HRESULT result = pDevice->CreateSamplerState( &desc, &pInternal );
        SM_ASSERT_MSG( result == S_OK, "D3D11SamplerState::Create > Failed to create sampler state" );
    }

    void D3D11Sampler::Destroy()
    {
        SAFE_RELEASE( pInternal );
    }
}