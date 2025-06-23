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
#include "dxgi_format.h"

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

    void DirectX11CommandList::Open()
    {
        ClearState();
    }

    void DirectX11CommandList::Close()
    {
        ClearState();
    }

    void DirectX11CommandList::ClearState()
    {
        m_Context.pImmediateContext->ClearState();

        m_IsCurrentGraphicsStateValid = false;

        m_CurrentGraphicsPipeline = GraphicsPipelineHandle::NullHandle();
        m_CurrentFramebuffer = FramebufferHandle::NullHandle();
        m_CurrentBindings.Resize( 0 );
        m_CurrentVertexBuffers.Resize( 0 );
        m_CurrentIndexBuffer = GPUBufferHandle::NullHandle();
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

    void DirectX11CommandList::SetGraphicsState( const GraphicsState &graphicsState )
    {
        const DirectX11Pipeline &pipeline = m_pDevice->m_Pipelines[graphicsState.Pipeline.GetIndex()];
        const DirectX11Pipeline &currentPipeline = m_pDevice->m_Pipelines[m_CurrentGraphicsPipeline.GetIndex()];
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

        if ( updateFramebuffer || currentPipeline.PixelShaderHasUAVs != pipeline.PixelShaderHasUAVs )
        {
            primitive::FixedVector< ID3D11RenderTargetView *, s_MaxRenderTargets > pRenderTargetsViews;

            for ( const auto &pRTV : framebuffer.pRenderTargetViews )
            {
                pRenderTargetsViews.PushBack( pRTV );
            }

            if ( pipeline.PixelShaderHasUAVs )
            {
                m_Context.pImmediateContext->OMSetRenderTargetsAndUnorderedAccessViews(
                    static_cast< UINT >( pRenderTargetsViews.GetCurrentItemCount() ),
                    pRenderTargetsViews.GetData(),
                    framebuffer.pDepthStencilView,
                    D3D11_KEEP_UNORDERED_ACCESS_VIEWS,
                    0,
                    nullptr,
                    nullptr );
            }
            else
            {
                m_Context.pImmediateContext->OMSetRenderTargets(
                    static_cast< UINT >( pRenderTargetsViews.GetCurrentItemCount() ),
                    pRenderTargetsViews.GetData(),
                    framebuffer.pDepthStencilView );
            }
        }

        if ( updatePipeline )
        {
            BindGraphicsPipeline( pipeline );
        }

        if ( updateBindings )
        {
            BindGraphicsResourceSets( setsToBind, pipeline );

            if ( pipeline.PixelShaderHasUAVs )
            {
                primitive::Array< ID3D11UnorderedAccessView *, D3D11_1_UAV_SLOT_COUNT > pUnorderedAccessViews{};
                static const primitive::Array< UINT, D3D11_1_UAV_SLOT_COUNT > initialCounts{};

                Uint32 minUAVSlot = D3D11_1_UAV_SLOT_COUNT;
                Uint32 maxUAVSlot = 0;

                for ( BindingSetHandle bindingSetHandle : graphicsState.Bindings )
                {
                    SM_ASSERT( bindingSetHandle.IsValid() );
                    SM_ASSERT( m_pDevice->m_BindingSets.IsValidIndex( bindingSetHandle.GetIndex() ) );

                    const DirectX11BindingSet &bindingSet = m_pDevice->m_BindingSets[bindingSetHandle.GetIndex()];

                    if ( !bindingSet.Visibility.Has( ShaderStage::Pixel ) )
                        continue;

                    for ( Uint32 slot = bindingSet.MinUAVSlot; slot <= bindingSet.MaxUAVSlot; ++slot )
                    {
                        pUnorderedAccessViews[slot] = bindingSet.pUnorderedAccessViews[slot];
                    }

                    minUAVSlot = std::min( minUAVSlot, bindingSet.MinUAVSlot );
                    maxUAVSlot = std::max( maxUAVSlot, bindingSet.MaxUAVSlot );
                }

                m_Context.pImmediateContext->OMSetRenderTargetsAndUnorderedAccessViews(
                    D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
                    nullptr,
                    nullptr,
                    minUAVSlot,
                    maxUAVSlot - minUAVSlot + 1,
                    pUnorderedAccessViews.GetData() + minUAVSlot,
                    initialCounts.GetData() );
            }
        }

        if ( updateVertexBuffers )
        {
            primitive::Array< ID3D11Buffer *, s_MaxVertexAttributeCount > pVertexBuffers{};
            primitive::Array< UINT, s_MaxVertexAttributeCount > vertexBufferStrides{};
            primitive::Array< UINT, s_MaxVertexAttributeCount > vertexBufferOffsets{};
            Uint32 maxVertexBufferIndex = 0;

            const auto &vertexElements = pipeline.Layout.GetElements();

            for ( auto i = 0; i < graphicsState.VertexBuffers.GetCurrentItemCount(); ++i )
            {
                const VertexBufferBinding &binding = graphicsState.VertexBuffers[i];

                if ( binding.Slot >= s_MaxVertexAttributeCount )
                    continue;

                SM_ASSERT( binding.Offset <= std::numeric_limits< Uint64 >::max() );

                SM_ASSERT( binding.VertexBuffer.IsValid() );
                SM_ASSERT( m_pDevice->m_GPUBuffers.IsValidIndex( binding.VertexBuffer.GetIndex() ) );

                pVertexBuffers[binding.Slot] = m_pDevice->m_GPUBuffers[binding.VertexBuffer.GetIndex()].pInternal;
                vertexBufferStrides[binding.Slot] = static_cast< UINT >( vertexElements.at( binding.Slot ).Size );
                vertexBufferOffsets[binding.Slot] = static_cast< UINT >( binding.Offset );
                maxVertexBufferIndex = std::max( maxVertexBufferIndex, binding.Slot );
            }

            if ( m_IsCurrentGraphicsStateValid )
            {
                for ( const VertexBufferBinding &binding : m_CurrentVertexBufferBindings )
                {
                    if ( binding.Slot < s_MaxVertexAttributeCount )
                        maxVertexBufferIndex = std::max( maxVertexBufferIndex, binding.Slot );
                }
            }

            m_Context.pImmediateContext->IASetVertexBuffers( 0,
                maxVertexBufferIndex + 1,
                pVertexBuffers.GetData(),
                vertexBufferStrides.GetData(),
                vertexBufferOffsets.GetData() );
        }

        if ( updateIndexBuffer )
        {
            if ( graphicsState.IndexBuffer.IndexBuffer.IsValid() )
            {
                SM_ASSERT( m_pDevice->m_GPUBuffers.IsValidIndex( graphicsState.IndexBuffer.IndexBuffer.GetIndex() ) );

                m_Context.pImmediateContext->IASetIndexBuffer(
                    m_pDevice->m_GPUBuffers[graphicsState.IndexBuffer.IndexBuffer.GetIndex()].pInternal,
                    GetDXGIFormatMapping( graphicsState.IndexBuffer.BufferFormat ).SRVFormat,
                    graphicsState.IndexBuffer.Offset );
            }
            else
            {
                m_Context.pImmediateContext->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
            }
        }

        m_IsCurrentGraphicsStateValid = true;
        if ( updatePipeline || updateFramebuffer || updateBindings || updateVertexBuffers || updateIndexBuffer )
        {
            m_CurrentGraphicsPipeline = graphicsState.Pipeline;
            m_CurrentFramebuffer = graphicsState.Framebuffer;

            m_CurrentBindings.Resize( graphicsState.Bindings.GetCurrentItemCount() );
            for ( auto i = 0; i < graphicsState.Bindings.GetCurrentItemCount(); ++i )
            {
                m_CurrentBindings[i] = graphicsState.Bindings[i];
            }

            m_CurrentVertexBufferBindings = graphicsState.VertexBuffers;
            m_CurrentIndexBufferBinding = graphicsState.IndexBuffer;

            m_CurrentVertexBuffers.Resize( graphicsState.VertexBuffers.GetCurrentItemCount() );
            for ( auto i = 0; i < graphicsState.VertexBuffers.GetCurrentItemCount(); ++i )
            {
                m_CurrentVertexBuffers[i] = graphicsState.VertexBuffers[i].VertexBuffer;
            }

            m_CurrentIndexBuffer = graphicsState.IndexBuffer.IndexBuffer;
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

    void DirectX11CommandList::FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const
    {
        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        m_Context.pImmediateContext->Map( gpuBuffer.pInternal, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource );
        memcpy( mappedResource.pData, pData, size );
        m_Context.pImmediateContext->Unmap( gpuBuffer.pInternal, 0 );
    }

    void *DirectX11CommandList::ReadTexture( TextureHandle handle )
    {
        auto &texture = m_pDevice->m_Textures[handle.GetIndex()];

        return texture.GetOrCreateShaderResourceView( m_Context.pDevice,
            texture.Descriptor.TextureFormat,
            TextureSubresourceSet{},
            texture.Descriptor.Dimension );
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

        SM_ASSERT_MSG( currentPipelineHandle.IsValid(),
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

    void DirectX11CommandList::BindGraphicsPipeline( const DirectX11Pipeline &pipeline ) const
    {
        m_Context.pImmediateContext->IASetInputLayout( pipeline.pInputLayout );

        m_Context.pImmediateContext->IASetPrimitiveTopology( pipeline.PrimitiveTopology );
        m_Context.pImmediateContext->RSSetState( pipeline.pRasterizerState );
        m_Context.pImmediateContext->OMSetDepthStencilState( pipeline.pDepthStencilState, 1 );

        m_Context.pImmediateContext->VSSetShader( pipeline.pVertexShader.Get(), nullptr, 0 );
        m_Context.pImmediateContext->PSSetShader( pipeline.pPixelShader.Get(), nullptr, 0 );
    }

    void DirectX11CommandList::BindGraphicsResourceSets( const BindingSetVector &setsToBind,
        const DirectX11Pipeline &pipeline ) const
    {
        for ( BindingSetHandle setHandle : setsToBind )
        {
            SM_ASSERT( m_pDevice->m_BindingSets.IsValidIndex( setHandle.GetIndex() ) );

            const DirectX11BindingSet &set = m_pDevice->m_BindingSets[setHandle.GetIndex()];

            foundation::Flags< ShaderStage > stagesToBind{ set.Visibility & pipeline.ShaderMask };

            if ( stagesToBind.Has( ShaderStage::Vertex ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->VSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - ( set.MinConstantBufferSlot + 1 ),
                        set.pConstantBuffers.GetData() );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->VSSetShaderResources(
                        set.MinSRVSlot, set.MaxSRVSlot - ( set.MinSRVSlot + 1 ), set.pShaderResourceViews.GetData() );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->VSSetSamplers(
                        set.MinSamplerSlot, set.MaxSamplerSlot - ( set.MinSamplerSlot + 1 ), set.pSamplers.GetData() );
                }
            }

            if ( stagesToBind.Has( ShaderStage::Pixel ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->PSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - ( set.MinConstantBufferSlot + 1 ),
                        set.pConstantBuffers.GetData() );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->PSSetShaderResources(
                        set.MinSRVSlot, set.MaxSRVSlot - ( set.MinSRVSlot + 1 ), set.pShaderResourceViews.GetData() );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->PSSetSamplers(
                        set.MinSamplerSlot, set.MaxSamplerSlot - ( set.MinSamplerSlot + 1 ), set.pSamplers.GetData() );
                }
            }
        }
    }
}