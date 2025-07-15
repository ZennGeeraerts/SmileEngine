/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_device.h"
#include "directx11_command_list.h"

#include "directx11_diagnostics.h"
#include "directx11_swap_chain.h"
#include "dxgi_format.h"

#include "smile/core/window/window.h"

namespace smile::graphic::rhi
{
    DirectX11Device::DirectX11Device()
    {
        // Create DXGI Factory to create SwapChain based on hardware
        HRESULT result =
            CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &m_Context.pDXGIFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11Device > Failed to create DXGIFactory: {}", fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }

        // Create Device and Device context, using hardware acceleration
        D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
        Uint32 createDeviceFlags{ 0 };
#ifdef SM_C_DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        result = D3D11CreateDevice( 0,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            createDeviceFlags,
            0,
            0,
            D3D11_SDK_VERSION,
            &m_Context.pDevice,
            &featureLevel,
            &m_Context.pImmediateContext );

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11Device > Failed to create D3D11Device: {}", fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }

        m_pImmediateCommandList = CreateScope< DirectX11CommandList >( this, std::ref( m_Context ) );
    }

    DirectX11Device::~DirectX11Device()
    {
        if ( m_Context.pImmediateContext )
        {
            m_Context.pImmediateContext->ClearState();
            m_Context.pImmediateContext->Flush();
            SAFE_RELEASE( m_Context.pImmediateContext );
        }

        SAFE_RELEASE( m_Context.pDevice );
    }

    CommandList *DirectX11Device::CreateCommandList()
    {
        return m_pImmediateCommandList.get();
    }

    memory::Ref< SwapChain > DirectX11Device::CreateSwapChain( const window::Window *pWindow )
    {
        auto pSwapChain = memory::CreateRef< DirectX11SwapChain >(
            pWindow, m_Context.pDevice, m_Context.pImmediateContext, m_Context.pDXGIFactory );
        pSwapChain->Create();
        return pSwapChain;
    }

    void DirectX11Device::CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc, void *pData )
    {
        m_GPUBuffers[handle.GetIndex()].Create( m_Context.pDevice, bufferDesc, pData );
    }

    void DirectX11Device::DestroyGPUBuffer( GPUBufferHandle handle )
    {
        m_GPUBuffers[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::CreateBindingSet( BindingSetHandle handle,
        const BindingSetDescriptor &bindingSetDesc,
        const BindingLayout &layout )
    {
        m_BindingSets[handle.GetIndex()].Create( *this, bindingSetDesc, layout );
    }

    void DirectX11Device::DestroyBindingSet( BindingSetHandle handle )
    {
        m_BindingSets[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::CreateShader( ShaderHandle handle,
        const ShaderDescriptor &shaderDesc,
        const std::vector< Byte > &byteCode )
    {
        m_Shaders[handle.GetIndex()].Create( m_Context.pDevice, shaderDesc, byteCode );
    }

    void DirectX11Device::DestroyShader( ShaderHandle handle )
    {
        m_Shaders[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::CreateGraphicsPipeline( GraphicsPipelineHandle handle,
        const GraphicsPipelineDescriptor &pipelineDesc )
    {
        m_Pipelines[handle.GetIndex()].Create( *this, pipelineDesc );
    }

    void DirectX11Device::DestroyGraphicsPipeline( GraphicsPipelineHandle handle )
    {
    }

    void DirectX11Device::CreateTexture( TextureHandle handle,
        const TextureDescriptor &desc,
        const std::vector< Byte > &buffer )
    {
        m_Textures[handle.GetIndex()].Create( m_Context.pDevice, desc, buffer );
    }

    void DirectX11Device::DestroyTexture( TextureHandle handle )
    {
        m_Textures[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::CreateStagingTexture( StagingTextureHandle handle, const TextureDescriptor &desc )
    {
        m_StagingTextures[handle.GetIndex()].Create( m_Context, desc );
    }

    void DirectX11Device::DestroyStagingTexture( StagingTextureHandle handle )
    {
        m_StagingTextures[handle.GetIndex()].Destroy();
    }

    void *DirectX11Device::MapStagingTexture( StagingTextureHandle handle,
        const TextureSlice &slice,
        CPUAccessMode cpuAccess )
    {
        SM_ASSERT( handle.IsValid() && m_StagingTextures.IsValidIndex( handle.GetIndex() ) );

        return m_StagingTextures[handle.GetIndex()].Map( m_Context, slice, cpuAccess );
    }

    void DirectX11Device::UnmapStagingTexture( StagingTextureHandle handle )
    {
        SM_ASSERT( handle.IsValid() && m_StagingTextures.IsValidIndex( handle.GetIndex() ) );

        m_StagingTextures[handle.GetIndex()].Unmap( m_Context );
    }

    Object DirectX11Device::GetNativeView( TextureHandle handle,
        ObjectType type,
        Format format,
        const TextureSubresourceSet &subresources,
        TextureDimension dimension,
        bool isReadOnlyDSV )
    {
        SM_ASSERT( handle.IsValid() && m_Textures.IsValidIndex( handle.GetIndex() ) );

        auto &texture = m_Textures[handle.GetIndex()];

        switch ( type )
        {
            case ObjectType::D3D11_RenderTargetView:
                return texture.GetOrCreateRenderTargetView( m_Context.pDevice, format, subresources );
            case ObjectType::D3D11_DepthStencilView:
                return texture.GetOrCreateDepthStencilView( m_Context.pDevice, subresources, isReadOnlyDSV );
            case ObjectType::D3D11_ShaderResourceView:
                return texture.GetOrCreateShaderResourceView( m_Context.pDevice, format, subresources, dimension );
            case ObjectType::D3D11_UnorderedAccessView:
                return texture.GetOrCreateUnorderedAccessView( m_Context.pDevice, format, subresources, dimension );
            default:
                return nullptr;
        }
    }

    void DirectX11Device::CreateSampler( SamplerHandle handle, const SamplerDescriptor &samplerDesc )
    {
        m_Samplers[handle.GetIndex()].Create( m_Context.pDevice, samplerDesc );
    }

    void DirectX11Device::DestroySampler( SamplerHandle handle )
    {
        m_Samplers[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor )
    {
        m_Framebuffers[handle.GetIndex()].Create( *this, descriptor );
    }

    void DirectX11Device::DestroyFramebuffer( FramebufferHandle handle )
    {
        m_Framebuffers[handle.GetIndex()].Destroy();
    }

    const DirectX11RasterizerState *DirectX11Device::GetOrCreateRasterizerState(
        const RasterizerState &rasterizerState )
    {
        const DirectX11RasterizerState *pRasterizerState = m_RasterizerStateCache.Find( rasterizerState );
        if ( pRasterizerState )
            return pRasterizerState;

        auto pNewRasterizerState = CreateScope< DirectX11RasterizerState >();
        pNewRasterizerState->Create( m_Context.pDevice, rasterizerState );
        return m_RasterizerStateCache.Add( rasterizerState, std::move( pNewRasterizerState ) );
    }

    const DirectX11DepthStencilState *DirectX11Device::GetOrCreateDepthStencilState(
        const DepthStencilState &depthStencilState )
    {
        const DirectX11DepthStencilState *pDepthStencilState = m_DepthStencilStateCache.Find( depthStencilState );
        if ( pDepthStencilState )
            return pDepthStencilState;

        auto pNewDepthStencilState = CreateScope< DirectX11DepthStencilState >();
        pNewDepthStencilState->Create( m_Context.pDevice, depthStencilState );
        return m_DepthStencilStateCache.Add( depthStencilState, std::move( pNewDepthStencilState ) );
    }

    const DirectX11InputLayout *DirectX11Device::GetOrCreateInputLayout(
        const GraphicsPipelineDescriptor &pipelineDesc )
    {
        const DirectX11InputLayout *pInputLayout = m_InputLayoutCache.Find( pipelineDesc.InputLayout );
        if ( pInputLayout )
            return pInputLayout;

        auto pNewInputLayout = CreateScope< DirectX11InputLayout >();
        pNewInputLayout->Create(
            m_Context.pDevice, pipelineDesc.InputLayout, m_Shaders[pipelineDesc.VertexShaderHandle.GetIndex()] );

        return m_InputLayoutCache.Add( pipelineDesc.InputLayout, std::move( pNewInputLayout ) );
    }

    ID3D11RenderTargetView *DirectX11Device::GetOrCreateRenderTargetViewForAttachment(
        const FramebufferAttachment &attachment )
    {
        SM_ASSERT( attachment.Texture.IsValid() && m_Textures.IsValidIndex( attachment.Texture.GetIndex() ) );

        auto &texture = m_Textures[attachment.Texture.GetIndex()];

        return texture.GetOrCreateRenderTargetView(
            m_Context.pDevice, attachment.TextureDesc.TextureFormat, attachment.Subresources );
    }

    ID3D11DepthStencilView *DirectX11Device::GetOrCreateDepthStencilViewForAttachment(
        const FramebufferAttachment &attachment )
    {
        SM_ASSERT( attachment.Texture.IsValid() && m_Textures.IsValidIndex( attachment.Texture.GetIndex() ) );

        auto &texture = m_Textures[attachment.Texture.GetIndex()];

        return texture.GetOrCreateDepthStencilView(
            m_Context.pDevice, attachment.Subresources, attachment.TextureDesc.CPUAccess == CPUAccessMode::Read );
    }
}