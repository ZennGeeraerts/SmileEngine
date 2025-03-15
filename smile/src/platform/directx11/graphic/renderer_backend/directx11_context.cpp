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

#include "smile/core/window/window.h"
#include "smile/common/foundation/compiled.h"
#include "smile/common/logging/logging.h"

namespace smile::graphic
{
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

    DirectX11Context::DirectX11Context( DirectX11Device *pDevice, ID3D11DeviceContext *pInternal )
        : m_pDevice{ pDevice }, m_pInternal{ pInternal }
    {
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

    void DirectX11Context::SetState( const RenderState &state ) const
    {
        D3D11_PRIMITIVE_TOPOLOGY directX11PrimitiveTopology = ConvertToDirectX11PrimitiveTopology( state.Topology );
        m_pInternal->IASetPrimitiveTopology( directX11PrimitiveTopology );

        const auto pRasterizerState = m_pDevice->GetOrCreateRasterizerState( state );
        m_pInternal->RSSetState( pRasterizerState->pInternal );

        const auto pDepthStencilState = m_pDevice->GetOrCreateDepthStencilState( state );
        m_pInternal->OMSetDepthStencilState( pDepthStencilState->pInternal, 1 );
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

    void DirectX11Context::BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        Uint32 offset{ 0 };
        m_pInternal->IASetVertexBuffers( 0, 1, &gpuBuffer.pInternal, &stride, &offset );
    }

    void DirectX11Context::UnbindVertexBuffer() const
    {
        m_pInternal->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }

    void DirectX11Context::BindIndexBuffer( GPUBufferHandle handle ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        m_pInternal->IASetIndexBuffer( gpuBuffer.pInternal, DXGI_FORMAT_R32_UINT, 0 );
    }

    void DirectX11Context::UnbindIndexBuffer() const
    {
        m_pInternal->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    }

    void DirectX11Context::BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        m_pInternal->VSSetConstantBuffers( slot, 1, &gpuBuffer.pInternal );
    }

    void DirectX11Context::UnbindVertexShaderUniformBuffer( Uint16 slot ) const
    {
        m_pInternal->VSSetConstantBuffers( slot, 1, nullptr );
    }

    void DirectX11Context::BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        m_pInternal->PSSetConstantBuffers( slot, 1, &gpuBuffer.pInternal );
    }

    void DirectX11Context::UnbindPixelShaderUniformBuffer( Uint16 slot ) const
    {
        m_pInternal->PSGetConstantBuffers( slot, 1, nullptr );
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

    void DirectX11Context::FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        m_pInternal->Map( gpuBuffer.pInternal, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource );
        memcpy( mappedResource.pData, pData, size );
        m_pInternal->Unmap( gpuBuffer.pInternal, 0 );
    }
}