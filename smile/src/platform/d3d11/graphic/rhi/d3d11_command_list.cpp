/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "d3d11_command_list.h"

#include "smile/graphic/rhi/swap_chain.h"
#include "smile/graphic/rhi/shader/shader.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"
#include "platform/d3dcommon/graphic/rhi/dxgi_format.h"
#include "d3d11_device.h"
#include "d3d11_swap_chain.h"
#include "d3d11_viewport_state.h"

#include "resource/d3d11_frame_buffer.h"

#include "smile/core/window/window.h"
#include "smile/common/foundation/compiled.h"
#include "smile/common/logging/logging.h"

namespace smile::graphic::rhi
{
    static ID3D11Buffer *s_NullConstantBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT]{ nullptr };
    static ID3D11ShaderResourceView *s_NullSRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{ nullptr };
    static ID3D11SamplerState *s_NullSamplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT]{ nullptr };

    D3D11CommandList::D3D11CommandList( D3D11Device *pDevice, std::reference_wrapper< const D3D11Context > context )
        : m_pDevice{ pDevice }, m_Context{ context.get() }
    {
    }

    Object D3D11CommandList::GetInternal( ObjectType type ) const
    {
        switch ( type )
        {
            case ObjectType::D3D11_DeviceContext:
                return Object{ m_Context.pImmediateContext };
            default:
                return nullptr;
        }
    }

    void D3D11CommandList::Open()
    {
        ClearState();
    }

    void D3D11CommandList::Close()
    {
        ClearState();
    }

    void D3D11CommandList::ClearState()
    {
        m_Context.pImmediateContext->ClearState();

        m_IsCurrentGraphicsStateValid = false;

        m_CurrentGraphicsPipeline = GraphicsPipelineHandle::NullHandle();
        m_CurrentFramebuffer = FramebufferHandle::NullHandle();
        m_CurrentBindings.Resize( 0 );
        m_CurrentVertexBuffers.Resize( 0 );
        m_CurrentIndexBuffer = GPUBufferHandle::NullHandle();
    }

    void D3D11CommandList::SetGraphicsState( const GraphicsState &graphicsState )
    {
        const D3D11Pipeline &pipeline = m_pDevice->m_Pipelines[graphicsState.Pipeline.GetIndex()];
        const D3D11Pipeline *pCurrentPipeline = m_CurrentGraphicsPipeline != GraphicsPipelineHandle::NullHandle()
                                                    ? &m_pDevice->m_Pipelines[m_CurrentGraphicsPipeline.GetIndex()]
                                                    : nullptr;
        const D3D11Framebuffer &framebuffer = m_pDevice->m_Framebuffers[graphicsState.Framebuffer.GetIndex()];

        const bool updateFramebuffer =
            !m_IsCurrentGraphicsStateValid || m_CurrentFramebuffer != graphicsState.Framebuffer;
        const bool updatePipeline =
            !m_IsCurrentGraphicsStateValid || m_CurrentGraphicsPipeline != graphicsState.Pipeline;
        const bool updateBindings = updateFramebuffer || m_CurrentBindings != graphicsState.Bindings;

        const bool updateViewports = !m_IsCurrentGraphicsStateValid || m_CurrentViewport != graphicsState.Viewport;

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

        if ( updateFramebuffer || pCurrentPipeline->PixelShaderHasUAVs != pipeline.PixelShaderHasUAVs )
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

                    const D3D11BindingSet &bindingSet = m_pDevice->m_BindingSets[bindingSetHandle.GetIndex()];

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

        if ( updateViewports )
        {
            D3D11ViewportState vpState{ graphicsState.Viewport };

            if ( vpState.ViewportCount )
            {
                m_Context.pImmediateContext->RSSetViewports( vpState.ViewportCount, vpState.Viewports.GetData() );
            }

            if ( vpState.ScissorRectCount )
            {
                m_Context.pImmediateContext->RSSetScissorRects(
                    vpState.ScissorRectCount, vpState.ScissorRects.GetData() );
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

                pVertexBuffers[binding.Slot] = m_pDevice->m_GPUBuffers[binding.VertexBuffer.GetIndex()].pInternal.Get();
                vertexBufferStrides[binding.Slot] = static_cast< UINT >( pipeline.Layout.GetStride() );
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
                    m_pDevice->m_GPUBuffers[graphicsState.IndexBuffer.IndexBuffer.GetIndex()].pInternal.Get(),
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

    void D3D11CommandList::ClearTexture( TextureHandle handle,
        TextureSubresourceSet subresources,
        const math::Color &clearColor ) const
    {
        SM_ASSERT( m_pDevice->IsHandleValid( handle, m_pDevice->m_Textures ) );

        D3D11Texture &texture = m_pDevice->m_Textures[handle.GetIndex()];

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

    void D3D11CommandList::ClearDepthStencilTexture( TextureHandle handle,
        TextureSubresourceSet subresources,
        std::optional< float > depth,
        std::optional< Uint8 > stencil ) const
    {
        if ( !depth.has_value() && !stencil.has_value() )
            return;

        SM_ASSERT( m_pDevice->IsHandleValid( handle, m_pDevice->m_Textures ) );

        D3D11Texture &texture = m_pDevice->m_Textures[handle.GetIndex()];

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

    void D3D11CommandList::CopyTexture( TextureHandle dst,
        const TextureDescriptor &dstDesc,
        const TextureSlice &dstSlice,
        TextureHandle src,
        const TextureDescriptor &srcDesc,
        const TextureSlice &srcSlice ) const
    {
        SM_ASSERT( m_pDevice->IsHandleValid( dst, m_pDevice->m_Textures ) );
        SM_ASSERT( m_pDevice->IsHandleValid( src, m_pDevice->m_Textures ) );

        auto resolvedSrcSlice = srcSlice.Resolve( srcDesc );
        auto resolvedDstSlice = dstSlice.Resolve( dstDesc );

        SM_ASSERT( resolvedDstSlice.Width == resolvedSrcSlice.Width );
        SM_ASSERT( resolvedDstSlice.Height == resolvedSrcSlice.Height );

        UINT srcSubresource =
            D3D11CalcSubresource( resolvedSrcSlice.MipLevelCount, resolvedSrcSlice.Slice, srcDesc.MipLevelCount );

        UINT dstSubresource =
            D3D11CalcSubresource( resolvedDstSlice.MipLevelCount, resolvedDstSlice.Slice, dstDesc.MipLevelCount );

        D3D11_BOX srcBox{};
        srcBox.left = resolvedSrcSlice.x;
        srcBox.top = resolvedSrcSlice.y;
        srcBox.front = resolvedSrcSlice.z;
        srcBox.right = resolvedSrcSlice.x + resolvedSrcSlice.Width;
        srcBox.bottom = resolvedSrcSlice.y + resolvedSrcSlice.Height;
        srcBox.back = resolvedSrcSlice.z + resolvedSrcSlice.Depth;

        const D3D11Texture &srcTexture = m_pDevice->m_Textures[src.GetIndex()];
        const D3D11Texture &dstTexture = m_pDevice->m_Textures[dst.GetIndex()];

        m_Context.pImmediateContext->CopySubresourceRegion( dstTexture.pInternal.Get(),
            dstSubresource,
            resolvedDstSlice.x,
            resolvedDstSlice.y,
            resolvedDstSlice.z,
            srcTexture.pInternal.Get(),
            srcSubresource,
            &srcBox );
    }

    void D3D11CommandList::CopyTexture( TextureHandle dst,
        const TextureSlice &dstSlice,
        TextureHandle src,
        const TextureSlice &srcSlice ) const
    {
        SM_ASSERT( m_pDevice->IsHandleValid( dst, m_pDevice->m_Textures ) );
        SM_ASSERT( m_pDevice->IsHandleValid( src, m_pDevice->m_Textures ) );

        const D3D11Texture &srcTexture = m_pDevice->m_Textures[src.GetIndex()];
        const D3D11Texture &dstTexture = m_pDevice->m_Textures[dst.GetIndex()];

        CopyTexture( dst, dstTexture.Descriptor, dstSlice, src, srcTexture.Descriptor, srcSlice );
    }

    void D3D11CommandList::Draw( const DrawParams &params )
    {
        m_Context.pImmediateContext->Draw( params.VertexCount, params.VertexOffset );
    }

    void D3D11CommandList::DrawIndexed( const DrawIndexedParams &params )
    {
        m_Context.pImmediateContext->DrawIndexed( params.IndexCount, params.IndexOffset, params.VertexOffset );
    }

    void D3D11CommandList::DrawInstanced( const DrawInstancedParams &params )
    {
        m_Context.pImmediateContext->DrawInstanced(
            params.VertexCount, params.InstanceCount, params.VertexOffset, params.InstanceOffset );
    }

    void D3D11CommandList::DrawIndexedInstanced( const DrawIndexedInstancedParams &params )
    {
        m_Context.pImmediateContext->DrawIndexedInstanced(
            params.IndexCount, params.InstanceCount, params.IndexOffset, params.VertexOffset, params.InstanceOffset );
    }

    void D3D11CommandList::FillBuffer( GPUBufferHandle handle, const void *pData, Uint32 size, Uint32 offset ) const
    {
        SM_ASSERT( m_pDevice->IsHandleValid( handle, m_pDevice->m_GPUBuffers ) );

        const auto &gpuBuffer = m_pDevice->m_GPUBuffers[handle.GetIndex()];

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        m_Context.pImmediateContext->Map(
            gpuBuffer.pInternal.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource );
        memcpy( static_cast< Byte * >( mappedResource.pData ) + offset, pData, size );
        m_Context.pImmediateContext->Unmap( gpuBuffer.pInternal.Get(), 0 );
    }

    void D3D11CommandList::PrepareToBindGraphicsResourceSets( const BindingSetVector &resourceSets,
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
            "D3D11CommandList::PrepareToBindGraphicsResourceSets > No valid pipeline set" );

        const D3D11Pipeline &currentPipeline = m_pDevice->m_Pipelines[currentPipelineHandle.GetIndex()];
        const D3D11Pipeline &newPipeline = m_pDevice->m_Pipelines[newPipelineHandle.GetIndex()];

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

            const D3D11BindingSet &set = m_pDevice->m_BindingSets[setHandle.GetIndex()];

            foundation::Flags< ShaderStage > stagesToUnbind{ set.Visibility & currentPipeline.ShaderMask };

            if ( stagesToUnbind.Has( ShaderStage::Vertex ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->VSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - set.MinConstantBufferSlot + 1,
                        s_NullConstantBuffers );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->VSSetShaderResources(
                        set.MinSRVSlot, set.MaxSRVSlot - set.MinSRVSlot + 1, s_NullSRVs );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->VSSetSamplers(
                        set.MinSamplerSlot, set.MaxSamplerSlot - set.MinSamplerSlot + 1, s_NullSamplers );
                }
            }

            if ( stagesToUnbind.Has( ShaderStage::Pixel ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->PSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - set.MinConstantBufferSlot + 1,
                        s_NullConstantBuffers );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->PSSetShaderResources(
                        set.MinSRVSlot, set.MaxSRVSlot - set.MinSRVSlot + 1, s_NullSRVs );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->PSSetSamplers(
                        set.MinSamplerSlot, set.MaxSamplerSlot - set.MinSamplerSlot + 1, s_NullSamplers );
                }
            }
        }
    }

    void D3D11CommandList::BindGraphicsPipeline( const D3D11Pipeline &pipeline ) const
    {
        m_Context.pImmediateContext->IASetInputLayout( pipeline.pInputLayout );

        m_Context.pImmediateContext->IASetPrimitiveTopology( pipeline.PrimitiveTopology );
        m_Context.pImmediateContext->RSSetState( pipeline.pRasterizerState );
        m_Context.pImmediateContext->OMSetDepthStencilState( pipeline.pDepthStencilState, 1 );

        m_Context.pImmediateContext->VSSetShader( pipeline.pVertexShader.Get(), nullptr, 0 );
        m_Context.pImmediateContext->PSSetShader( pipeline.pPixelShader.Get(), nullptr, 0 );
    }

    void D3D11CommandList::BindGraphicsResourceSets( const BindingSetVector &setsToBind,
        const D3D11Pipeline &pipeline ) const
    {
        for ( BindingSetHandle setHandle : setsToBind )
        {
            SM_ASSERT( m_pDevice->m_BindingSets.IsValidIndex( setHandle.GetIndex() ) );

            const D3D11BindingSet &set = m_pDevice->m_BindingSets[setHandle.GetIndex()];

            foundation::Flags< ShaderStage > stagesToBind{ set.Visibility & pipeline.ShaderMask };

            if ( stagesToBind.Has( ShaderStage::Vertex ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->VSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - set.MinConstantBufferSlot + 1,
                        &set.pConstantBuffers.GetData()[set.MinConstantBufferSlot] );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->VSSetShaderResources( set.MinSRVSlot,
                        set.MaxSRVSlot - set.MinSRVSlot + 1,
                        &set.pShaderResourceViews.GetData()[set.MinSRVSlot] );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->VSSetSamplers( set.MinSamplerSlot,
                        set.MaxSamplerSlot - set.MinSamplerSlot + 1,
                        &set.pSamplers.GetData()[set.MinSamplerSlot] );
                }
            }

            if ( stagesToBind.Has( ShaderStage::Pixel ) )
            {
                if ( set.MaxConstantBufferSlot >= set.MinConstantBufferSlot )
                {
                    m_Context.pImmediateContext->PSSetConstantBuffers( set.MinConstantBufferSlot,
                        set.MaxConstantBufferSlot - set.MinConstantBufferSlot + 1,
                        &set.pConstantBuffers.GetData()[set.MinConstantBufferSlot] );
                }

                if ( set.MaxSRVSlot >= set.MinSRVSlot )
                {
                    m_Context.pImmediateContext->PSSetShaderResources( set.MinSRVSlot,
                        set.MaxSRVSlot - set.MinSRVSlot + 1,
                        &set.pShaderResourceViews.GetData()[set.MinSRVSlot] );
                }

                if ( set.MaxSamplerSlot >= set.MinSamplerSlot )
                {
                    m_Context.pImmediateContext->PSSetSamplers( set.MinSamplerSlot,
                        set.MaxSamplerSlot - set.MinSamplerSlot + 1,
                        &set.pSamplers.GetData()[set.MinSamplerSlot] );
                }
            }
        }
    }
}