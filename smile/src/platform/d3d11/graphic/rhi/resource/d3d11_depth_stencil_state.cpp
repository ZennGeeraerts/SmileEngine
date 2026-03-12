/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_depth_stencil_state.h"

namespace smile::graphic::rhi
{
    static D3D11_DEPTH_WRITE_MASK DepthWriteMaskToD3D11Type( DepthWriteMask depthWriteMask )
    {
        switch ( depthWriteMask )
        {
            case DepthWriteMask::Zero:
                return D3D11_DEPTH_WRITE_MASK_ZERO;

            case DepthWriteMask::All:
                return D3D11_DEPTH_WRITE_MASK_ALL;

            default:
                SM_ASSERT_MSG( false, "Unknown depth write mask" );
        }
    }

    static D3D11_COMPARISON_FUNC DepthComparissonFuncToD3D11Type( DepthComparissonFunc depthComparissonFunc )
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
                SM_ASSERT_MSG( false, "Unknown depth comparisson function" );
        }
    }

    D3D11DepthStencilState::~D3D11DepthStencilState()
    {
        Destroy();
    }

    void D3D11DepthStencilState::Create( ID3D11Device *pDevice, const DepthStencilState &depthStencilState )
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = depthStencilState.DepthEnable;
        depthStencilDesc.DepthWriteMask = DepthWriteMaskToD3D11Type( depthStencilState.DepthWriteMask );
        depthStencilDesc.DepthFunc = DepthComparissonFuncToD3D11Type( depthStencilState.DepthComparissonFunc );

        [[maybe_unused]] HRESULT result = pDevice->CreateDepthStencilState( &depthStencilDesc, &pInternal );
        SM_ASSERT_MSG( result == S_OK, "D3D11DepthStencilState::Create > Failed to create depth stencil state" );
    }

    void D3D11DepthStencilState::Destroy()
    {
        pInternal.Reset();
    }
}