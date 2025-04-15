/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_sampler_state.h"

namespace smile::graphic
{
    static D3D11_FILTER SamplerFilteringToDirectXType( SamplerFiltering filtering )
    {
        switch ( filtering )
        {
            case SamplerFiltering::MinMagMipPoint:
                return D3D11_FILTER_MIN_MAG_MIP_POINT;
            case SamplerFiltering::MinMagMipLinear:
                return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            default:
                SM_ASSERT( false, "Unknown filter" );
        }
    }

    static D3D11_TEXTURE_ADDRESS_MODE SamplerAddressingToDirectXType( SamplerAddressing addressing )
    {
        switch ( addressing )
        {
            case SamplerAddressing::Wrap:
                return D3D11_TEXTURE_ADDRESS_WRAP;
            case SamplerAddressing::Clamp:
                return D3D11_TEXTURE_ADDRESS_CLAMP;
            default:
                SM_ASSERT( false, "Unknown addressing" );
        }
    }

    DirectX11SamplerState::~DirectX11SamplerState()
    {
        Destroy();
    }

    void DirectX11SamplerState::Create( ID3D11Device *pDevice, const SamplerState &samplerState )
    {
        D3D11_SAMPLER_DESC desc{};
        desc.Filter = SamplerFilteringToDirectXType( samplerState.Filtering );
        desc.AddressU = SamplerAddressingToDirectXType( samplerState.AddressingU );
        desc.AddressV = SamplerAddressingToDirectXType( samplerState.AddressingV );
        desc.AddressW = SamplerAddressingToDirectXType( samplerState.AddressingW );

        HRESULT result = pDevice->CreateSamplerState( &desc, &pInternal );
        SM_ASSERT( result == S_OK, "DirectX11SamplerState::Create > Failed to create sampler state" );
    }

    void DirectX11SamplerState::Destroy()
    {
        SAFE_RELEASE( pInternal );
    }
}