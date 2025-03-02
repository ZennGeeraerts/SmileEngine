/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_context.h"

#include "directx11_device.h"
#include "directx11_diagnostics.h"

#include "resource/directx11_swap_chain.h"
#include "resource/directx11_frame_buffer.h"
#include "shader/directx11_shader.h"

#include "window/window.h"
#include "foundation/compiled.h"
#include "logging/logging.h"

namespace smile::graphic
{
    DirectX11Context::~DirectX11Context()
    {
        if ( m_pInternal )
        {
            m_pInternal->ClearState();
            m_pInternal->Flush();
            SAFE_RELEASE( m_pInternal );
        }
    }

    void DirectX11Context::BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const
    {
        auto pDX11SwapChain = memory::Ref< DirectX11SwapChain >{ pSwapChain };

        auto pRenderTargetView = pDX11SwapChain->GetRenderTargetView();
        m_pInternal->OMSetRenderTargets( 1, &pRenderTargetView, pDX11SwapChain->GetDepthStencilView() );
        m_pInternal->RSSetViewports( 1, &pDX11SwapChain->GetViewport() );
    }

    void DirectX11Context::ClearBackBuffer( memory::Ref< SwapChain > pSwapChain,
        const DirectX::XMFLOAT4 &clearColor ) const
    {
        auto pDX11SwapChain = memory::Ref< DirectX11SwapChain >{ pSwapChain };

        auto pRenderTargetView = pDX11SwapChain->GetRenderTargetView();
        m_pInternal->OMSetRenderTargets( 1, &pRenderTargetView, pDX11SwapChain->GetDepthStencilView() );

        const float *pClearColor = reinterpret_cast< const float * >( &clearColor );
        m_pInternal->ClearRenderTargetView( pRenderTargetView, pClearColor );
        m_pInternal->ClearDepthStencilView(
            pDX11SwapChain->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11Context::Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader )
    {
        auto pDirectX11Shader = memory::Ref< DirectX11Shader >{ pShader };
        SM_ASSERT( pDirectX11Shader, "DirectX11Context::Draw > Shader is not a DirectX11Shader" );

        auto pTechnique = pDirectX11Shader->pTechnique;
        D3DX11_TECHNIQUE_DESC techDesc{};
        pTechnique->GetDesc( &techDesc );
        for ( UINT p{}; p < techDesc.Passes; ++p )
        {
            pTechnique->GetPassByIndex( p )->Apply( 0, m_pInternal );
            m_pInternal->Draw( vertexCount, 0 );
        }
    }

    void DirectX11Context::DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader )
    {
        auto pDirectX11Shader = memory::Ref< DirectX11Shader >{ pShader };
        SM_ASSERT( pDirectX11Shader, "DirectX11RendererAPI::DrawIndexed > Shader is not a DirectX11Shader" );

        auto pTechnique = pDirectX11Shader->pTechnique;
        D3DX11_TECHNIQUE_DESC techDesc{};
        pTechnique->GetDesc( &techDesc );
        for ( UINT p{}; p < techDesc.Passes; ++p )
        {
            pTechnique->GetPassByIndex( p )->Apply( 0, m_pInternal );
            m_pInternal->DrawIndexed( indexCount, 0, 0 );
        }
    }

    void DirectX11Context::ClearFramebuffer( memory::Ref< Framebuffer > pFramebuffer )
    {
        const float *pClearColor = reinterpret_cast< const float * >( &pFramebuffer->ClearColor );
        auto pRenderTargetViews =
            static_cast< ID3D11RenderTargetView *const * >( pFramebuffer->GetRenderTargetViews() );
        auto pDepthStencilView = static_cast< ID3D11DepthStencilView * >( pFramebuffer->GetDepthStencilView() );

        for ( Uint32 i{}; i < pFramebuffer->GetRenderTargetViewCount(); ++i )
        {
            m_pInternal->ClearRenderTargetView( pRenderTargetViews[i], pClearColor );
        }

        if ( pFramebuffer->DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
            m_pInternal->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11Context::BindVertexBuffer( const memory::Ref< VertexBuffer > &pVertexBuffer ) const
    {
        Uint32 offset{ 0 };
        ID3D11Buffer *pInternalBuffer = static_cast< ID3D11Buffer * >( pVertexBuffer->GetInternal() );
        m_pInternal->IASetVertexBuffers( 0, 1, &pInternalBuffer, &pVertexBuffer->Stride, &offset );
    }

    void DirectX11Context::UnbindVertexBuffer() const
    {
        m_pInternal->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }

    void DirectX11Context::BindIndexBuffer( const memory::Ref< IndexBuffer > &pIndexBuffer ) const
    {
        ID3D11Buffer *pInternalBuffer = static_cast< ID3D11Buffer * >( pIndexBuffer->GetInternal() );
        m_pInternal->IASetIndexBuffer( pInternalBuffer, DXGI_FORMAT_R32_UINT, 0 );
    }

    void DirectX11Context::UnbindIndexBuffer() const
    {
        m_pInternal->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    }

    void DirectX11Context::BindShader( const memory::Ref< Shader > &pShader ) const
    {
        ID3D11InputLayout *pInputLayout = static_cast< ID3D11InputLayout * >( pShader->GetData() );
        m_pInternal->IASetInputLayout( pInputLayout );
    }

    void DirectX11Context::UnbindShader() const
    {
        m_pInternal->IASetInputLayout( nullptr );
    }

    void DirectX11Context::BindFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) const
    {
        auto pRenderTargetViews =
            static_cast< ID3D11RenderTargetView *const * >( pFramebuffer->GetRenderTargetViews() );
        auto pDepthStencilView = static_cast< ID3D11DepthStencilView * >( pFramebuffer->GetDepthStencilView() );
        auto pViewport = static_cast< D3D11_VIEWPORT * >( pFramebuffer->GetViewport() );

        m_pInternal->OMSetRenderTargets(
            pFramebuffer->GetRenderTargetViewCount(), &pRenderTargetViews[0], pDepthStencilView );
        m_pInternal->RSSetViewports( 1, pViewport );
    }

    void DirectX11Context::BindRasterizerState( const memory::Ref< RasterizerState > &pRasterizerState ) const
    {
        ID3D11RasterizerState *pD11RasterizerState =
            static_cast< ID3D11RasterizerState * >( pRasterizerState->GetInternal() );
        m_pInternal->RSSetState( pD11RasterizerState );
    }

    void DirectX11Context::UnbindRasterizerState() const
    {
        m_pInternal->RSSetState( nullptr );
    }

    static D3D11_PRIMITIVE_TOPOLOGY ConvertToDirectX11PrimitiveTopology( PrimitiveTopology primitiveTopology )
    {
        switch ( primitiveTopology )
        {
            case smile::graphic::PrimitiveTopology::None:
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
            case smile::graphic::PrimitiveTopology::TriangleList:
                return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case smile::graphic::PrimitiveTopology::LineList:
                return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            default:
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        }
    }

    void DirectX11Context::BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const
    {
        D3D11_PRIMITIVE_TOPOLOGY directX11PrimitiveTopology = ConvertToDirectX11PrimitiveTopology( primitiveTopology );
        m_pInternal->IASetPrimitiveTopology( directX11PrimitiveTopology );
    }

    void DirectX11Context::UnbindPrimitiveTopology() const
    {
        m_pInternal->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_UNDEFINED );
    }

    void DirectX11Context::FillVertexBuffer( const memory::Ref< VertexBuffer > &pVertexBuffer,
        void *pData,
        Uint32 vertexCount ) const
    {
        auto pDirectX11VertexBuffer = static_cast< ID3D11Buffer * >( pVertexBuffer->GetInternal() );

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        m_pInternal->Map( pDirectX11VertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource );

        if ( vertexCount > 0 )
        {
            memcpy( mappedResource.pData, pData, pVertexBuffer->Stride * vertexCount );
        }

        m_pInternal->Unmap( pDirectX11VertexBuffer, 0 );
    }
}