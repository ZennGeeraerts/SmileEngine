#include "smpch.h"
#include "directx11_rasterizer_state.h"

#include "smile_engine/core/application.h"
#include "smile_engine/core/logger.h"

#include "platform/directX11/directX11_diagnostics.h"

namespace smile::graphic
{
    static D3D11_CULL_MODE cullModeToDirectXType( CullMode cull_mode )
    {
        switch ( cull_mode )
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

    static D3D11_FILL_MODE fillModeToDirectXType( FillMode fill_mode )
    {
        switch ( fill_mode )
        {
            case FillMode::Solid:
                return D3D11_FILL_SOLID;
            case FillMode::WireFrame:
                return D3D11_FILL_WIREFRAME;
            default:
                return D3D11_FILL_SOLID;
        }
    }

    DirectX11RasterizerState::DirectX11RasterizerState( const RasterizerStateData &rasterizer_state_data )
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );

        D3D11_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.CullMode = cullModeToDirectXType( rasterizer_state_data.cullMode );
        rasterizer_desc.FillMode = fillModeToDirectXType( rasterizer_state_data.fillMode );
        rasterizer_desc.DepthClipEnable = rasterizer_state_data.depthClipEnable;

        HRESULT result =
            directX11Context->getDevice()->CreateRasterizerState( &rasterizer_desc, &rasterizerState );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RasterizerState::DirectX11RasterizerState > Failed to create rasterizer state: %ls",
                getDirectX11ErrorMessage( result ) );
        }
    }

    DirectX11RasterizerState::~DirectX11RasterizerState()
    {
        SAFE_RELEASE( rasterizerState );
    }

    void DirectX11RasterizerState::bind() const
    {
        directX11Context->getDeviceContext()->RSSetState( rasterizerState );
    }

    void DirectX11RasterizerState::unbind() const
    {
        directX11Context->getDeviceContext()->RSSetState( nullptr );
    }
}