/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "d3d11_rasterizer_state.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"

namespace smile::graphic::rhi
{
    static D3D11_CULL_MODE CullModeToD3D11Type( CullMode cullMode )
    {
        switch ( cullMode )
        {
            case CullMode::None:
                return D3D11_CULL_NONE;
            case CullMode::Front:
                return D3D11_CULL_FRONT;
            case CullMode::Back:
                return D3D11_CULL_BACK;
            default:
                return D3D11_CULL_NONE;
        }
    }

    static D3D11_FILL_MODE FillModeToD3D11Type( FillMode fillMode )
    {
        switch ( fillMode )
        {
            case FillMode::Solid:
                return D3D11_FILL_SOLID;
            case FillMode::WireFrame:
                return D3D11_FILL_WIREFRAME;
            default:
                return D3D11_FILL_SOLID;
        }
    }

    D3D11RasterizerState::~D3D11RasterizerState()
    {
        Destroy();
    }

    void D3D11RasterizerState::Create( ID3D11Device *pDevice, const RasterizerState &rasterizerState )
    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.CullMode = CullModeToD3D11Type( rasterizerState.CullMode );
        rasterizerDesc.FillMode = FillModeToD3D11Type( rasterizerState.FillMode );
        rasterizerDesc.DepthClipEnable = rasterizerState.EnableDepthClip;

        HRESULT result = pDevice->CreateRasterizerState( &rasterizerDesc, &pInternal );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11RasterizerState::Create > Failed to create rasterizer state: {}",
                GetD3DErrorMessageString( result ) );
        }
    }

    void D3D11RasterizerState::Destroy()
    {
        pInternal.Reset();
    }
}