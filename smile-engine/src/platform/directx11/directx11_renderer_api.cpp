#include "smpch.h"
#include "directx11_renderer_api.h"

#include "platform/directX11/shader/directx11_shader.h"
#include "smile_engine/core/application.h"

#include "directx11_diagnostics.h"

namespace smile::graphic
{
    void DirectX11RendererAPI::initialize()
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( directX11Context, "DirectX11RendererAPI > RenderingContext is not a DirectX11Context" );
    }

    void DirectX11RendererAPI::resizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        if ( !directX11Context->swapChain || !directX11Context->device ||
             !directX11Context->deviceContext )
            return;

        directX11Context->deviceContext->OMSetRenderTargets( 0, 0, 0 );

        D3D11_TEXTURE2D_DESC depth_stencil_desc{};
        directX11Context->depthStencilBuffer->GetDesc( &depth_stencil_desc );
        depth_stencil_desc.Width = width;
        depth_stencil_desc.Height = height;

        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
        directX11Context->depthStencilView->GetDesc( &depth_stencil_view_desc );

        SAFE_RELEASE( directX11Context->currentRenderTarget );
        SAFE_RELEASE( directX11Context->renderTargetBuffer );
        SAFE_RELEASE( directX11Context->depthStencilView );
        SAFE_RELEASE( directX11Context->depthStencilBuffer );

        HRESULT result = directX11Context->swapChain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::resizeWindow > Failed to resize buffers: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        // Depth stencil
        result = directX11Context->device->CreateTexture2D(
            &depth_stencil_desc, 0, &directX11Context->depthStencilBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::resizeWindow > Failed to create depth stencil buffer: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        result = directX11Context->device->CreateDepthStencilView( directX11Context->depthStencilBuffer,
            &depth_stencil_view_desc,
            &directX11Context->depthStencilView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::resizeWindow > Failed to create depth stencil view: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        // Render target
        result = directX11Context->swapChain->GetBuffer( 0,
            __uuidof( ID3D11Texture2D ),
            reinterpret_cast< void ** >( &directX11Context->renderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::resizeWindow > Failed to get buffer from swap chain: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        result = directX11Context->device->CreateRenderTargetView(
            directX11Context->renderTargetBuffer, 0, &directX11Context->currentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::resizeWindow > Failed to create render target view: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        directX11Context->deviceContext->OMSetRenderTargets(
            1, &directX11Context->currentRenderTarget, directX11Context->depthStencilView );

        directX11Context->viewport.Width = static_cast< FLOAT >( width );
        directX11Context->viewport.Height = static_cast< FLOAT >( height );
        directX11Context->viewport.TopLeftX = static_cast< FLOAT >( x );
        directX11Context->viewport.TopLeftY = static_cast< FLOAT >( y );

        directX11Context->deviceContext->RSSetViewports( 1, &directX11Context->viewport );
    }

    void DirectX11RendererAPI::setClearColor( const DirectX::XMFLOAT4 &color )
    {
        clearColor = color;
    }

    void DirectX11RendererAPI::clear()
    {
        directX11Context->deviceContext->OMSetRenderTargets(
            1, &directX11Context->currentRenderTarget, directX11Context->depthStencilView );

        const float *clear_color = reinterpret_cast< const float * >( &clearColor );
        directX11Context->deviceContext->ClearRenderTargetView(
            directX11Context->currentRenderTarget, clear_color );
        directX11Context->deviceContext->ClearDepthStencilView(
            directX11Context->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11RendererAPI::drawIndexed( Uint32 index_count, const Ref< Shader > &shader )
    {
        auto directX11_shader = static_cast< DirectX11Shader * >( shader.get() );
        SM_ASSERT( directX11_shader, "DirectX11RendererAPI::drawIndexed > Shader is not a DirectX11Shader" );

        directX11Context->deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        auto technique = directX11_shader->getTechnique();
        D3DX11_TECHNIQUE_DESC tech_desc{};
        technique->GetDesc( &tech_desc );
        for ( UINT p{}; p < tech_desc.Passes; ++p )
        {
            technique->GetPassByIndex( p )->Apply( 0, directX11Context->deviceContext );
            directX11Context->deviceContext->DrawIndexed( index_count, 0, 0 );
        }
    }
}