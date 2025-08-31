/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_rasterizer_state.h"

#include "platform/directx11/graphic/rhi/directx11_diagnostics.h"

namespace smile::graphic
{
    static D3D11_CULL_MODE CullModeToDirectXType( CullMode cullMode )
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

    static D3D11_FILL_MODE FillModeToDirectXType( FillMode fillMode )
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

    DirectX11RasterizerState::~DirectX11RasterizerState()
    {
        Destroy();
    }

    void DirectX11RasterizerState::Create( ID3D11Device *pDevice, const RenderState &renderState )
    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.CullMode = CullModeToDirectXType( renderState.CullMode );
        rasterizerDesc.FillMode = FillModeToDirectXType( renderState.FillMode );
        rasterizerDesc.DepthClipEnable = renderState.EnableDepthClip;

        HRESULT result = pDevice->CreateRasterizerState( &rasterizerDesc, &pInternal );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RasterizerState::Create > Failed to create rasterizer state: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }
    }

    void DirectX11RasterizerState::Destroy()
    {
        SAFE_RELEASE( pInternal );
    }
}