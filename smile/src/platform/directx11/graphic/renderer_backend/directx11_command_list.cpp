/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_command_list.h"

#include "smile/graphic/rhi/swap_chain.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"
#include "smile/graphic/rhi/shader/shader.h"

#include "directx11_device.h"
#include "directx11_diagnostics.h"
#include "directx11_swap_chain.h"

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

    DirectX11CommandList::DirectX11CommandList( DirectX11Device *pDevice,
        std::reference_wrapper< const DirectX11Context > context )
        : m_pDevice{ pDevice }, m_Context{ context.get() }
    {
    }

    void DirectX11CommandList::BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const
    {
        auto pDX11SwapChain = memory::Ref< DirectX11SwapChain >{ pSwapChain };

        auto pRenderTargetView = pDX11SwapChain->GetRenderTargetView();
        m_Context.pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, pDX11SwapChain->GetDepthStencilView() );
        m_Context.pImmediateContext->RSSetViewports( 1, &pDX11SwapChain->GetViewport() );
    }

    void DirectX11CommandList::ClearBackBuffer( memory::Ref< SwapChain > pSwapChain,
        const DirectX::XMFLOAT4 &clearColor ) const
    {
        auto pDX11SwapChain = memory::Ref< DirectX11SwapChain >{ pSwapChain };

        auto pRenderTargetView = pDX11SwapChain->GetRenderTargetView();
        m_Context.pImmediateContext->OMSetRenderTargets( 1, &pRenderTargetView, pDX11SwapChain->GetDepthStencilView() );

        const float *pClearColor = reinterpret_cast< const float * >( &clearColor );
        m_Context.pImmediateContext->ClearRenderTargetView( pRenderTargetView, pClearColor );
        m_Context.pImmediateContext->ClearDepthStencilView(
            pDX11SwapChain->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11CommandList::SetState( const RenderState &state ) const
    {
        D3D11_PRIMITIVE_TOPOLOGY directX11PrimitiveTopology = ConvertToDirectX11PrimitiveTopology( state.Topology );
        m_Context.pImmediateContext->IASetPrimitiveTopology( directX11PrimitiveTopology );

        const auto pRasterizerState = m_pDevice->GetOrCreateRasterizerState( state );
        m_Context.pImmediateContext->RSSetState( pRasterizerState->pInternal );

        const auto pDepthStencilState = m_pDevice->GetOrCreateDepthStencilState( state );
        m_Context.pImmediateContext->OMSetDepthStencilState( pDepthStencilState->pInternal, 1 );
    }

    void DirectX11CommandList::SetVertexShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const
    {
        const auto pSamplerState = m_pDevice->GetOrCreateSamplerState( samplerState );
        m_Context.pImmediateContext->VSSetSamplers( slot, 1, &pSamplerState->pInternal );
    }

    void DirectX11CommandList::SetPixelShaderSamplerState( const SamplerState &samplerState, Uint16 slot ) const
    {
        const auto pSamplerState = m_pDevice->GetOrCreateSamplerState( samplerState );
        m_Context.pImmediateContext->PSSetSamplers( slot, 1, &pSamplerState->pInternal );
    }

    void DirectX11CommandList::Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader )
    {
        auto pDirectX11Shader = memory::Ref< DirectX11Shader >{ pShader };
        SM_ASSERT_MSG( pDirectX11Shader, "DirectX11Context::Draw > Shader is not a DirectX11Shader" );

        auto pTechnique = pDirectX11Shader->pTechnique;
        D3DX11_TECHNIQUE_DESC techDesc{};
        pTechnique->GetDesc( &techDesc );
        for ( UINT p{}; p < techDesc.Passes; ++p )
        {
            pTechnique->GetPassByIndex( p )->Apply( 0, m_Context.pImmediateContext );
            m_Context.pImmediateContext->Draw( vertexCount, 0 );
        }
    }

    void DirectX11CommandList::DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader )
    {
        auto pDirectX11Shader = memory::Ref< DirectX11Shader >{ pShader };
        SM_ASSERT_MSG( pDirectX11Shader, "DirectX11RendererAPI::DrawIndexed > Shader is not a DirectX11Shader" );

        auto pTechnique = pDirectX11Shader->pTechnique;
        D3DX11_TECHNIQUE_DESC techDesc{};
        pTechnique->GetDesc( &techDesc );
        for ( UINT p{}; p < techDesc.Passes; ++p )
        {
            pTechnique->GetPassByIndex( p )->Apply( 0, m_Context.pImmediateContext );
            m_Context.pImmediateContext->DrawIndexed( indexCount, 0, 0 );
        }
    }

    void DirectX11CommandList::ClearFramebuffer( FramebufferHandle handle )
    {
        const auto &framebuffer = m_pDevice->m_Framebuffers[handle.GetIndex()];

        const float *pClearColor = reinterpret_cast< const float * >( &framebuffer.Descriptor.ClearColor );
        ID3D11RenderTargetView *const *pRenderTargetViews = framebuffer.pRenderTargetViews.data();

        for ( size_t i{}; i < framebuffer.pRenderTargetViews.size(); ++i )
        {
            m_Context.pImmediateContext->ClearRenderTargetView( pRenderTargetViews[i], pClearColor );
        }

        if ( framebuffer.DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
            m_Context.pImmediateContext->ClearDepthStencilView(
                framebuffer.pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11CommandList::BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        Uint32 offset{ 0 };
        m_Context.pImmediateContext->IASetVertexBuffers( 0, 1, &gpuBuffer.pInternal, &stride, &offset );
    }

    void DirectX11CommandList::UnbindVertexBuffer() const
    {
        m_Context.pImmediateContext->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }

    void DirectX11CommandList::BindIndexBuffer( GPUBufferHandle handle ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        m_Context.pImmediateContext->IASetIndexBuffer( gpuBuffer.pInternal, DXGI_FORMAT_R32_UINT, 0 );
    }

    void DirectX11CommandList::UnbindIndexBuffer() const
    {
        m_Context.pImmediateContext->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    }

    void DirectX11CommandList::BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        m_Context.pImmediateContext->VSSetConstantBuffers( slot, 1, &gpuBuffer.pInternal );
    }

    void DirectX11CommandList::UnbindVertexShaderUniformBuffer( Uint16 slot ) const
    {
        m_Context.pImmediateContext->VSSetConstantBuffers( slot, 1, nullptr );
    }

    void DirectX11CommandList::BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];
        m_Context.pImmediateContext->PSSetConstantBuffers( slot, 1, &gpuBuffer.pInternal );
    }

    void DirectX11CommandList::UnbindPixelShaderUniformBuffer( Uint16 slot ) const
    {
        m_Context.pImmediateContext->PSGetConstantBuffers( slot, 1, nullptr );
    }

    void DirectX11CommandList::BindShader( const memory::Ref< Shader > &pShader ) const
    {
        ID3D11InputLayout *pInputLayout = static_cast< ID3D11InputLayout * >( pShader->GetData() );
        m_Context.pImmediateContext->IASetInputLayout( pInputLayout );
    }

    void DirectX11CommandList::UnbindShader() const
    {
        m_Context.pImmediateContext->IASetInputLayout( nullptr );
    }

    void DirectX11CommandList::BindFramebuffer( FramebufferHandle handle ) const
    {
        const auto &framebuffer = m_pDevice->m_Framebuffers[handle.GetIndex()];

        ID3D11RenderTargetView *const *pRenderTargetViews = framebuffer.pRenderTargetViews.data();

        m_Context.pImmediateContext->OMSetRenderTargets( static_cast< UINT >( framebuffer.pRenderTargetViews.size() ),
            &pRenderTargetViews[0],
            framebuffer.pDepthStencilView );

        m_Context.pImmediateContext->RSSetViewports( 1, &framebuffer.Viewport );
    }

    void DirectX11CommandList::FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        m_Context.pImmediateContext->Map( gpuBuffer.pInternal, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource );
        memcpy( mappedResource.pData, pData, size );
        m_Context.pImmediateContext->Unmap( gpuBuffer.pInternal, 0 );
    }

    void *DirectX11CommandList::ReadTexture( TextureHandle handle ) const
    {
        const auto &texture = m_pDevice->m_Textures[handle.GetIndex()];
        return texture.pShaderResourceView;
    }

    void *DirectX11CommandList::ReadTexture( FramebufferHandle handle, Uint32 index ) const
    {
        const auto &framebuffer = m_pDevice->m_Framebuffers[handle.GetIndex()];

        SM_ASSERT_MSG( index < framebuffer.pColorShaderResourceViews.size(),
            "DirectX11Context::ReadTexture > Index out of range" );

        return framebuffer.pColorShaderResourceViews[index];
    }
}