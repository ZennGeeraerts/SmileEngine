/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_command_list.h"

#include "smile/graphic/renderer_backend/swap_chain.h"
#include "smile/graphic/renderer_backend/resource/frame_buffer.h"
#include "smile/graphic/renderer_backend/shader/shader.h"

#include "directx11_device.h"
#include "directx11_diagnostics.h"
#include "directx11_swap_chain.h"

#include "resource/directx11_frame_buffer.h"

#include "smile/core/window/window.h"
#include "smile/common/foundation/compiled.h"
#include "smile/common/logging/logging.h"

namespace smile::graphic
{
    static ID3D11Buffer *s_NullConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]{ nullptr };
    static ID3D11ShaderResourceView *s_NullSRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{ nullptr };
    static ID3D11SamplerState *s_NullSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT]{ nullptr };

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

    void DirectX11CommandList::BindGraphicsPipeline( GraphicsPipelineHandle handle ) const
    {
        const auto &pipeline = m_pDevice->m_Pipelines[handle.GetIndex()];

        m_Context.pImmediateContext->IASetInputLayout( pipeline.pInputLayout );

        m_Context.pImmediateContext->IASetPrimitiveTopology( pipeline.PrimitiveTopology );
        m_Context.pImmediateContext->RSSetState( pipeline.pRasterizerState );
        m_Context.pImmediateContext->OMSetDepthStencilState( pipeline.pDepthStencilState, 1 );

        m_Context.pImmediateContext->VSSetShader( pipeline.pVertexShader.Get(), nullptr, 0 );
        m_Context.pImmediateContext->PSSetShader( pipeline.pPixelShader.Get(), nullptr, 0 );
    }

    void DirectX11CommandList::SetGraphicsState( const GraphicsState &graphicsState ) const
    {
        const DirectX11Pipeline &pipeline = m_pDevice->m_Pipelines[graphicsState.Pipeline.GetIndex()];
        const DirectX11Framebuffer &framebuffer = m_pDevice->m_Framebuffers[graphicsState.Framebuffer.GetIndex()];

        const bool updateFramebuffer =
            !m_IsCurrentGraphicsStateValid || m_CurrentFramebuffer != graphicsState.Framebuffer;
        const bool updatePipeline =
            !m_IsCurrentGraphicsStateValid || m_CurrentGraphicsPipeline != graphicsState.Pipeline;
        const bool updateBindings = updateFramebuffer || m_CurrentBindings != graphicsState.Bindings;

        const bool updateIndexBuffer =
            !m_IsCurrentGraphicsStateValid || m_CurrentIndexBufferBinding != graphicsState.IndexBuffer;
        const bool updateVertexBuffers =
            !m_IsCurrentGraphicsStateValid || m_CurrentVertexBufferBindings != graphicsState.VertexBuffers;

        BindingSetVector setsToBind;
        if ( updateBindings )
        {
            PrepareToBindGraphicsResourceSets( graphicsState.Bindings,
                m_IsCurrentGraphicsStateValid ? &m_CurrentBindings : nullptr,
                m_CurrentGraphicsPipeline,
                graphicsState.Pipeline,
                updateFramebuffer,
                setsToBind );
        }
    }

    void DirectX11CommandList::Draw( const DrawParams &params )
    {
        m_Context.pImmediateContext->Draw( params.VertexCount, params.VertexOffset );
    }

    void DirectX11CommandList::DrawIndexed( const DrawIndexedParams &params )
    {
        m_Context.pImmediateContext->DrawIndexed( params.IndexCount, params.IndexOffset, params.VertexOffset );
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

    void DirectX11CommandList::PrepareToBindGraphicsResourceSets( const BindingSetVector &resourceSets,
        const BindingSetVector *pCurrentResourceSets,
        GraphicsPipelineHandle currentPipelineHandle,
        GraphicsPipelineHandle newPipelineHandle,
        bool updateFramebuffer,
        BindingSetVector &outSetsToBind ) const
    {
        outSetsToBind = resourceSets;

        if ( !pCurrentResourceSets )
            return;

        SM_ASSERT( currentPipelineHandle.IsValid(),
            "DirectX11CommandList::PrepareToBindGraphicsResourceSets > No valid pipeline set" );

        const DirectX11Pipeline &currentPipeline = m_pDevice->m_Pipelines[currentPipelineHandle.GetIndex()];
        const DirectX11Pipeline &newPipeline = m_pDevice->m_Pipelines[newPipelineHandle.GetIndex()];

        std::vector< BindingSetHandle > setsToUnbind;

        for ( const BindingSetHandle &bindingSet : *pCurrentResourceSets )
        {
            setsToUnbind.push_back( bindingSet );
        }

        if ( currentPipeline.ShaderMask == newPipeline.ShaderMask )
        {
            for ( auto &outSetToBind : outSetsToBind )
            {
                if ( !outSetToBind.IsValid() )
                    continue;

                for ( auto &setToUnbind : setsToUnbind )
                {
                    if ( outSetToBind == setToUnbind )
                    {
                        outSetToBind = BindingSetHandle::NullHandle();
                        setToUnbind = BindingSetHandle::NullHandle();
                        break;
                    }
                }
            }

            if ( !updateFramebuffer )
            {
                for ( auto &outSetToBind : outSetsToBind )
                {
                    if ( !outSetToBind.IsValid() )
                        continue;

                    for ( auto &setToUnbind : setsToUnbind )
                    {
                        if ( setToUnbind.IsValid() && m_pDevice->m_BindingSets[outSetToBind.GetIndex()].IsSuperSetOf(
                                                          m_pDevice->m_BindingSets[setToUnbind.GetIndex()] ) )
                        {
                            setToUnbind = BindingSetHandle::NullHandle();
                        }
                    }
                }
            }
        }

        for ( const BindingSetHandle &setHandle : setsToUnbind )
        {
            if ( !setHandle.IsValid() )
                continue;

            const DirectX11BindingSet &set = m_pDevice->m_BindingSets[setHandle.GetIndex()];

            foundation::Flags< ShaderStage > stagesToUnbind{ set.Visibility & currentPipeline.ShaderMask };

            if ( stagesToUnbind.Has( ShaderStage::Vertex ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->VSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - ( set.MinConstantBufferSlot + 1 ),
                        s_NullConstantBuffers );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->VSSetShaderResources(
                        set.MinSRVSlot, set.MaxSRVSlot - ( set.MinSRVSlot + 1 ), s_NullSRVs );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->VSSetSamplers(
                        set.MinSamplerSlot, set.MaxSamplerSlot - ( set.MinSamplerSlot + 1 ), s_NullSamplers );
                }
            }

            if ( stagesToUnbind.Has( ShaderStage::Pixel ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->PSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - ( set.MinConstantBufferSlot + 1 ),
                        s_NullConstantBuffers );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->PSSetShaderResources(
                        set.MinSRVSlot, set.MaxSRVSlot - ( set.MinSRVSlot + 1 ), s_NullSRVs );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->PSSetSamplers(
                        set.MinSamplerSlot, set.MaxSamplerSlot - ( set.MinSamplerSlot + 1 ), s_NullSamplers );
                }
            }
        }
    }
}