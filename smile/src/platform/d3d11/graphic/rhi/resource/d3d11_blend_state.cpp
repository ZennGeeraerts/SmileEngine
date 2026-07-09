/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        d3d11_blend_state.cpp
 * @author      Zenn Geeraerts
 * @created     9 July 2026
 * @brief       D3D11 implementation of a blend state
 */
#include "smpch.h"
#include "d3d11_blend_state.h"

namespace smile::graphic::rhi
{
    static D3D11_BLEND BlendFactorToD3D11Type( BlendFactor blendFactor )
    {
        switch ( blendFactor )
        {
            case BlendFactor::Zero:
                return D3D11_BLEND_ZERO;

            case BlendFactor::One:
                return D3D11_BLEND_ONE;

            case BlendFactor::SrcColor:
                return D3D11_BLEND_SRC_COLOR;

            case BlendFactor::InvSrcColor:
                return D3D11_BLEND_INV_SRC_COLOR;

            case BlendFactor::SrcAlpha:
                return D3D11_BLEND_SRC_ALPHA;

            case BlendFactor::InvSrcAlpha:
                return D3D11_BLEND_INV_SRC_ALPHA;

            case BlendFactor::DestAlpha:
                return D3D11_BLEND_DEST_ALPHA;

            case BlendFactor::InvDestAlpha:
                return D3D11_BLEND_INV_DEST_ALPHA;

            case BlendFactor::DestColor:
                return D3D11_BLEND_DEST_COLOR;

            case BlendFactor::InvDestColor:
                return D3D11_BLEND_INV_DEST_COLOR;

            case BlendFactor::SrcAlphaSaturate:
                return D3D11_BLEND_SRC_ALPHA_SAT;

            case BlendFactor::ConstantColor:
                return D3D11_BLEND_BLEND_FACTOR;

            case BlendFactor::InvConstantColor:
                return D3D11_BLEND_INV_BLEND_FACTOR;

            case BlendFactor::Src1Color:
                return D3D11_BLEND_SRC1_COLOR;

            case BlendFactor::InvSrc1Color:
                return D3D11_BLEND_INV_SRC1_COLOR;

            case BlendFactor::Src1Alpha:
                return D3D11_BLEND_SRC1_ALPHA;

            case BlendFactor::InvSrc1Alpha:
                return D3D11_BLEND_INV_SRC1_ALPHA;

            default:
                SM_ASSERT_MSG( false, "Unknown blend factor" );
        }
    }

    static D3D11_BLEND_OP BlendOpToD3D11Type( BlendOp blendOp )
    {
        switch ( blendOp )
        {
            case BlendOp::Add:
                return D3D11_BLEND_OP_ADD;

            case BlendOp::Subtract:
                return D3D11_BLEND_OP_SUBTRACT;

            case BlendOp::RevSubtract:
                return D3D11_BLEND_OP_REV_SUBTRACT;

            case BlendOp::Min:
                return D3D11_BLEND_OP_MIN;

            case BlendOp::Max:
                return D3D11_BLEND_OP_MAX;

            default:
                SM_ASSERT_MSG( false, "Unknown blend operation" );
        }
    }

    D3D11BlendState::~D3D11BlendState()
    {
        Destroy();
    }

    void D3D11BlendState::Create( ID3D11Device *pDevice, const BlendState &blendState )
    {
        SM_ASSERT( pDevice );

        D3D11_BLEND_DESC desc{};
        desc.AlphaToCoverageEnable = blendState.AlphaToCoverageEnable;
        desc.IndependentBlendEnable = TRUE;

        for ( Index i{ 0 }; i < s_MaxRenderTargets; ++i )
        {
            const auto &target = blendState.Targets[i];

            D3D11_RENDER_TARGET_BLEND_DESC &rtDesc = desc.RenderTarget[i];
            rtDesc.BlendEnable = target.BlendEnabled;
            rtDesc.SrcBlend = BlendFactorToD3D11Type( target.SrcBlend );
            rtDesc.DestBlend = BlendFactorToD3D11Type( target.DestBlend );
            rtDesc.BlendOp = BlendOpToD3D11Type( target.BlendOp_ );
            rtDesc.SrcBlendAlpha = BlendFactorToD3D11Type( target.SrcBlendAlpha );
            rtDesc.DestBlendAlpha = BlendFactorToD3D11Type( target.DestBlendAlpha );
            rtDesc.BlendOpAlpha = BlendOpToD3D11Type( target.BlendOpAlpha );
            rtDesc.RenderTargetWriteMask = static_cast< UINT8 >( target.ColorWriteMask );
        }

        HRESULT hr = pDevice->CreateBlendState( &desc, pInternal.GetAddressOf() );
        SM_ASSERT_MSG( SUCCEEDED( hr ), "Failed to create D3D11 blend state." );
    }

    void D3D11BlendState::Destroy()
    {
        pInternal.Reset();
    }
}