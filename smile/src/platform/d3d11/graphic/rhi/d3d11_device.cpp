/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "d3d11_device.h"
#include "d3d11_command_list.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"
#include "platform/d3dcommon/graphic/rhi/dxgi_format.h"
#include "d3d11_swap_chain.h"

#include "smile/core/window/window.h"

namespace smile::graphic::rhi
{
    D3D11Device::D3D11Device()
    {
        // Create DXGI Factory to create SwapChain based on hardware
        HRESULT result =
            CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &m_Context.pDXGIFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11Device > Failed to create DXGIFactory: {}", GetD3DErrorMessageString( result ) );
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
            SM_LOG_ERROR( "D3D11Device > Failed to create D3D11Device: {}", GetD3DErrorMessageString( result ) );
            return;
        }

        m_pImmediateCommandList = CreateScope< D3D11CommandList >( this, std::ref( m_Context ) );
    }

    D3D11Device::~D3D11Device()
    {
        if ( m_Context.pImmediateContext )
        {
            m_Context.pImmediateContext->ClearState();
            m_Context.pImmediateContext->Flush();
            SAFE_RELEASE( m_Context.pImmediateContext );
        }

        SAFE_RELEASE( m_Context.pDevice );
    }

    Object D3D11Device::GetInternal( ObjectType type ) const
    {
        switch ( type )
        {
            case ObjectType::D3D11_Device:
                return Object{ m_Context.pDevice };
            case ObjectType::D3D11_DeviceContext:
                return Object{ m_Context.pImmediateContext };
            default:
                return nullptr;
        }
    }

    CommandList *D3D11Device::CreateCommandList()
    {
        return m_pImmediateCommandList.get();
    }

    Scope< SwapChain > D3D11Device::CreateSwapChain( const window::Window *pWindow )
    {
        auto pSwapChain = CreateScope< D3D11SwapChain >(
            pWindow, m_Context.pDevice, m_Context.pImmediateContext, m_Context.pDXGIFactory );
        pSwapChain->Create();
        return pSwapChain;
    }

    void D3D11Device::CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc, void *pData )
    {
        m_GPUBuffers[handle.GetIndex()].Create( m_Context.pDevice, bufferDesc, pData );
    }

    void D3D11Device::DestroyGPUBuffer( GPUBufferHandle handle )
    {
        m_GPUBuffers[handle.GetIndex()].Destroy();
    }

    void D3D11Device::CreateBindingLayout( BindingLayoutHandle handle, const BindingLayout &layout )
    {
        m_BindingLayouts[handle.GetIndex()].Create( layout );
    }

    void D3D11Device::DestroyBindingLayout( BindingLayoutHandle handle )
    {
    }

    void D3D11Device::CreateBindingSet( BindingSetHandle handle,
        const BindingSetDescriptor &bindingSetDesc,
        BindingLayoutHandle layoutHandle )
    {
        SM_ASSERT( IsHandleValid( layoutHandle, m_BindingLayouts ) );

        const auto &bindingLayout = m_BindingLayouts[layoutHandle.GetIndex()];

        m_BindingSets[handle.GetIndex()].Create( *this, bindingSetDesc, bindingLayout.Internal );
    }

    void D3D11Device::DestroyBindingSet( BindingSetHandle handle )
    {
        m_BindingSets[handle.GetIndex()].Destroy();
    }

    void D3D11Device::CreateShader( ShaderHandle handle,
        const ShaderDescriptor &shaderDesc,
        const primitive::Vector< Byte > &byteCode )
    {
        m_Shaders[handle.GetIndex()].Create( m_Context.pDevice, shaderDesc, byteCode );
    }

    void D3D11Device::DestroyShader( ShaderHandle handle )
    {
        m_Shaders[handle.GetIndex()].Destroy();
    }

    void D3D11Device::CreateGraphicsPipeline( GraphicsPipelineHandle handle,
        const GraphicsPipelineDescriptor &pipelineDesc )
    {
        m_Pipelines[handle.GetIndex()].Create( *this, pipelineDesc );
    }

    void D3D11Device::DestroyGraphicsPipeline( GraphicsPipelineHandle handle )
    {
    }

    void D3D11Device::CreateTexture( TextureHandle handle,
        const TextureDescriptor &desc,
        const primitive::Vector< Byte > &buffer )
    {
        m_Textures[handle.GetIndex()].Create( m_Context.pDevice, desc, buffer );
    }

    void D3D11Device::DestroyTexture( TextureHandle handle )
    {
        m_Textures[handle.GetIndex()].Destroy();
    }

    void D3D11Device::CreateStagingTexture( StagingTextureHandle handle, const TextureDescriptor &desc )
    {
        m_StagingTextures[handle.GetIndex()].Create( m_Context, desc );
    }

    void D3D11Device::DestroyStagingTexture( StagingTextureHandle handle )
    {
        m_StagingTextures[handle.GetIndex()].Destroy();
    }

    void *
    D3D11Device::MapStagingTexture( StagingTextureHandle handle, const TextureSlice &slice, CPUAccessMode cpuAccess )
    {
        SM_ASSERT( IsHandleValid( handle, m_StagingTextures ) );

        return m_StagingTextures[handle.GetIndex()].Map( m_Context, slice, cpuAccess );
    }

    void D3D11Device::UnmapStagingTexture( StagingTextureHandle handle )
    {
        SM_ASSERT( IsHandleValid( handle, m_StagingTextures ) );

        m_StagingTextures[handle.GetIndex()].Unmap( m_Context );
    }

    void D3D11Device::CreateHandleForNativeTexture( TextureHandle handle,
        Object nativeTexture,
        ObjectType type,
        const TextureDescriptor &desc )
    {
        SM_ASSERT( IsHandleValid( handle, m_Textures ) );

        if ( !nativeTexture.Pointer || type != ObjectType::D3D11_Resource )
            return;

        D3D11Texture &texture = m_Textures[handle.GetIndex()];
        texture.pInternal = static_cast< ID3D11Resource * >( nativeTexture.Pointer );
        texture.Descriptor = desc;
    }

    Object D3D11Device::GetNativeView( TextureHandle handle,
        ObjectType type,
        Format format,
        const TextureSubresourceSet &subresources,
        TextureDimension dimension,
        bool isReadOnlyDSV )
    {
        SM_ASSERT( IsHandleValid( handle, m_Textures ) );

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

    void D3D11Device::CreateSampler( SamplerHandle handle, const SamplerDescriptor &samplerDesc )
    {
        m_Samplers[handle.GetIndex()].Create( m_Context.pDevice, samplerDesc );
    }

    void D3D11Device::DestroySampler( SamplerHandle handle )
    {
        m_Samplers[handle.GetIndex()].Destroy();
    }

    void D3D11Device::CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor )
    {
        m_Framebuffers[handle.GetIndex()].Create( *this, descriptor );
    }

    void D3D11Device::DestroyFramebuffer( FramebufferHandle handle )
    {
        m_Framebuffers[handle.GetIndex()].Destroy();
    }

    const D3D11RasterizerState *D3D11Device::GetOrCreateRasterizerState( const RasterizerState &rasterizerState )
    {
        const D3D11RasterizerState *pRasterizerState = m_RasterizerStateCache.Find( rasterizerState );
        if ( pRasterizerState )
            return pRasterizerState;

        auto pNewRasterizerState = CreateScope< D3D11RasterizerState >();
        pNewRasterizerState->Create( m_Context.pDevice, rasterizerState );
        return m_RasterizerStateCache.Add( rasterizerState, std::move( pNewRasterizerState ) );
    }

    const D3D11DepthStencilState *D3D11Device::GetOrCreateDepthStencilState(
        const DepthStencilState &depthStencilState )
    {
        const D3D11DepthStencilState *pDepthStencilState = m_DepthStencilStateCache.Find( depthStencilState );
        if ( pDepthStencilState )
            return pDepthStencilState;

        auto pNewDepthStencilState = CreateScope< D3D11DepthStencilState >();
        pNewDepthStencilState->Create( m_Context.pDevice, depthStencilState );
        return m_DepthStencilStateCache.Add( depthStencilState, std::move( pNewDepthStencilState ) );
    }

    const D3D11InputLayout *D3D11Device::GetOrCreateInputLayout( const GraphicsPipelineDescriptor &pipelineDesc )
    {
        const D3D11InputLayout *pInputLayout = m_InputLayoutCache.Find( pipelineDesc.InputLayout );
        if ( pInputLayout )
            return pInputLayout;

        auto pNewInputLayout = CreateScope< D3D11InputLayout >();
        pNewInputLayout->Create(
            m_Context.pDevice, pipelineDesc.InputLayout, m_Shaders[pipelineDesc.VertexShaderHandle.GetIndex()] );

        return m_InputLayoutCache.Add( pipelineDesc.InputLayout, std::move( pNewInputLayout ) );
    }

    ID3D11RenderTargetView *D3D11Device::GetOrCreateRenderTargetViewForAttachment(
        const FramebufferAttachment &attachment )
    {
        SM_ASSERT( IsHandleValid( attachment.Texture, m_Textures ) );

        auto &texture = m_Textures[attachment.Texture.GetIndex()];

        return texture.GetOrCreateRenderTargetView(
            m_Context.pDevice, attachment.TextureDesc.TextureFormat, attachment.Subresources );
    }

    ID3D11DepthStencilView *D3D11Device::GetOrCreateDepthStencilViewForAttachment(
        const FramebufferAttachment &attachment )
    {
        SM_ASSERT( IsHandleValid( attachment.Texture, m_Textures ) );

        auto &texture = m_Textures[attachment.Texture.GetIndex()];

        return texture.GetOrCreateDepthStencilView(
            m_Context.pDevice, attachment.Subresources, attachment.TextureDesc.CPUAccess == CPUAccessMode::Read );
    }
}