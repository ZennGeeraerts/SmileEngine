/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_command_list.h"

#include "smile/graphic/rhi/swap_chain.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"
#include "smile/graphic/rhi/shader/shader.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"
#include "directx11_device.h"
#include "directx11_swap_chain.h"
#include "dxgi_format.h"

#include "resource/directx11_frame_buffer.h"

#include "smile/core/window/window.h"
#include "smile/common/foundation/compiled.h"
#include "smile/common/logging/logging.h"

namespace smile::graphic::rhi
{
    static ID3D11Buffer *s_NullConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]{ nullptr };
    static ID3D11ShaderResourceView *s_NullSRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{ nullptr };
    static ID3D11SamplerState *s_NullSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT]{ nullptr };

    DirectX11CommandList::DirectX11CommandList( DirectX11Device *pDevice,
        std::reference_wrapper< const DirectX11Context > context )
        : m_pDevice{ pDevice }, m_Context{ context.get() }
    {
    }

    Object DirectX11CommandList::GetInternal( ObjectType type ) const
    {
        switch ( type )
        {
            case ObjectType::D3D11_DeviceContext:
                return Object{ m_Context.pImmediateContext };
            default:
                return nullptr;
        }
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
                pRenderTargetsViews.PushBack( pRTV.Get() );
            }

            if ( pipeline.PixelShaderHasUAVs )
            {
                m_Context.pImmediateContext->OMSetRenderTargetsAndUnorderedAccessViews(
                    static_cast< UINT >( pRenderTargetsViews.GetItemCount() ),
                    pRenderTargetsViews.GetData(),
                    framebuffer.pDepthStencilView.Get(),
                    D3D11_KEEP_UNORDERED_ACCESS_VIEWS,
                    0,
                    nullptr,
                    nullptr );
            }
            else
            {
                m_Context.pImmediateContext->OMSetRenderTargets(
                    static_cast< UINT >( pRenderTargetsViews.GetItemCount() ),
                    pRenderTargetsViews.GetData(),
                    framebuffer.pDepthStencilView.Get() );
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

            for ( auto i = 0; i < graphicsState.VertexBuffers.GetItemCount(); ++i )
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

            m_CurrentBindings.Resize( graphicsState.Bindings.GetItemCount() );
            for ( auto i = 0; i < graphicsState.Bindings.GetItemCount(); ++i )
            {
                m_CurrentBindings[i] = graphicsState.Bindings[i];
            }

            m_CurrentVertexBufferBindings = graphicsState.VertexBuffers;
            m_CurrentIndexBufferBinding = graphicsState.IndexBuffer;

            m_CurrentVertexBuffers.Resize( graphicsState.VertexBuffers.GetItemCount() );
            for ( auto i = 0; i < graphicsState.VertexBuffers.GetItemCount(); ++i )
            {
                m_CurrentVertexBuffers[i] = graphicsState.VertexBuffers[i].VertexBuffer;
            }

            m_CurrentIndexBuffer = graphicsState.IndexBuffer.IndexBuffer;
        }
    }

    void DirectX11CommandList::ClearTexture( TextureHandle handle,
        TextureSubresourceSet subresources,
        const math::Color &clearColor ) const
    {
        SM_ASSERT( m_pDevice->IsHandleValid( handle, m_pDevice->m_Textures ) );

        DirectX11Texture &texture = m_pDevice->m_Textures[handle.GetIndex()];

#ifdef SM_C_DEBUG
        const FormatInfo &formatInfo = GetFormatInfo( texture.Descriptor.TextureFormat );
        SM_ASSERT( !formatInfo.HasDepth && !formatInfo.HasStencil );
        SM_ASSERT( texture.Descriptor.BindFlags.HasAny(
            { TextureBindFlags::UnorderedAccess, TextureBindFlags::RenderTarget } ) );
#endif

        subresources = subresources.Resolve( texture.Descriptor, false );

        for ( const MipmapLevel mipLevel : foundation::GetRangeIterator(
                  subresources.BaseMipLevel, subresources.BaseMipLevel + subresources.MipLevelCount ) )
        {
            auto currentMipSlice =
                TextureSubresourceSet{ mipLevel, 1, subresources.BaseArraySlice, subresources.ArraySliceCount };

            if ( texture.Descriptor.BindFlags.Has( TextureBindFlags::UnorderedAccess ) )
            {
                ID3D11UnorderedAccessView *pUAV = texture.GetOrCreateUnorderedAccessView(
                    m_Context.pDevice, Format::UNKNOWN, currentMipSlice, TextureDimension::Unknown );

                m_Context.pImmediateContext->ClearUnorderedAccessViewFloat( pUAV, &clearColor.r );
            }
            else if ( texture.Descriptor.BindFlags.Has( TextureBindFlags::RenderTarget ) )
            {
                ID3D11RenderTargetView *pRTV =
                    texture.GetOrCreateRenderTargetView( m_Context.pDevice, Format::UNKNOWN, currentMipSlice );

                m_Context.pImmediateContext->ClearRenderTargetView( pRTV, &clearColor.r );
            }
            else
            {
                break;
            }
        }
    }

    void DirectX11CommandList::ClearDepthStencilTexture( TextureHandle handle,
        TextureSubresourceSet subresources,
        std::optional< float > depth,
        std::optional< Uint8 > stencil ) const
    {
        if ( !depth.has_value() && !stencil.has_value() )
            return;

        SM_ASSERT( m_pDevice->IsHandleValid( handle, m_pDevice->m_Textures ) );

        DirectX11Texture &texture = m_pDevice->m_Textures[handle.GetIndex()];

#ifdef SM_C_DEBUG
        const FormatInfo &formatInfo = GetFormatInfo( texture.Descriptor.TextureFormat );
        SM_ASSERT( texture.Descriptor.BindFlags.Has( TextureBindFlags::RenderTarget ) );
        SM_ASSERT( formatInfo.HasDepth || formatInfo.HasStencil );
#endif

        subresources = subresources.Resolve( texture.Descriptor, false );

        for ( const MipmapLevel mipLevel : foundation::GetRangeIterator(
                  subresources.BaseMipLevel, subresources.BaseMipLevel + subresources.MipLevelCount ) )
        {
            auto currentMipSlice =
                TextureSubresourceSet{ mipLevel, 1, subresources.BaseArraySlice, subresources.ArraySliceCount };

            ID3D11DepthStencilView *pDSV = texture.GetOrCreateDepthStencilView( m_Context.pDevice, currentMipSlice );

            UINT clearFlags = 0;
            if ( depth.has_value() )
                clearFlags |= D3D11_CLEAR_DEPTH;
            if ( stencil.has_value() )
                clearFlags |= D3D11_CLEAR_STENCIL;

            m_Context.pImmediateContext->ClearDepthStencilView(
                pDSV, clearFlags, depth.value_or( 1.0f ), stencil.value_or( 0 ) );
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