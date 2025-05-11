/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_depth_stencil_state.h"

namespace smile::graphic
{
    static D3D11_DEPTH_WRITE_MASK DepthWriteMaskToDirectXType( DepthWriteMask depthWriteMask )
    {
        switch ( depthWriteMask )
        {
            case DepthWriteMask::Zero:
                return D3D11_DEPTH_WRITE_MASK_ZERO;

            case DepthWriteMask::All:
                return D3D11_DEPTH_WRITE_MASK_ALL;

            default:
                SM_ASSERT( false, "Unknown depth write mask" );
        }
    }

    static D3D11_COMPARISON_FUNC DepthComparissonFuncToDirectXType( DepthComparissonFunc depthComparissonFunc )
    {
        switch ( depthComparissonFunc )
        {
            case DepthComparissonFunc::Never:
                return D3D11_COMPARISON_NEVER;

            case DepthComparissonFunc::Less:
                return D3D11_COMPARISON_LESS;

            case DepthComparissonFunc::Equal:
                return D3D11_COMPARISON_EQUAL;

            case DepthComparissonFunc::LessEqual:
                return D3D11_COMPARISON_LESS_EQUAL;

            case DepthComparissonFunc::Greater:
                return D3D11_COMPARISON_GREATER;

            case DepthComparissonFunc::NotEqual:
                return D3D11_COMPARISON_NOT_EQUAL;

            case DepthComparissonFunc::GreaterEqual:
                return D3D11_COMPARISON_GREATER_EQUAL;

            case DepthComparissonFunc::Always:
                return D3D11_COMPARISON_ALWAYS;

            default:
                SM_ASSERT( false, "Unknown depth comparisson function" );
        }
    }

    DirectX11DepthStencilState::~DirectX11DepthStencilState()
    {
        Destroy();
    }

    void DirectX11DepthStencilState::Create( ID3D11Device *pDevice, const DepthStencilState &depthStencilState )
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = depthStencilState.DepthEnable;
        depthStencilDesc.DepthWriteMask = DepthWriteMaskToDirectXType( depthStencilState.DepthWriteMask );
        depthStencilDesc.DepthFunc = DepthComparissonFuncToDirectXType( depthStencilState.DepthComparissonFunc );

        [[maybe_unused]] HRESULT result = pDevice->CreateDepthStencilState( &depthStencilDesc, &pInternal );
        SM_ASSERT( result == S_OK, "DirectX11DepthStencilState::Create > Failed to create depth stencil state" );
    }

    void DirectX11DepthStencilState::Destroy()
    {
        SAFE_RELEASE( pInternal );
    }
}