/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "resource_manager.h"

#include "smile/graphic/rhi/graphics_device.h"

namespace smile::graphic
{
    ResourceManager::~ResourceManager()
    {
        for ( auto pVertexBuffer : m_pVertexBuffers )
            m_pDevice->DestroyGPUBuffer( pVertexBuffer->m_Handle );

        for ( auto pIndexBuffer : m_pIndexBuffers )
            m_pDevice->DestroyGPUBuffer( pIndexBuffer->m_Handle );

        for ( auto pConstantBuffer : m_pConstantBuffers )
            m_pDevice->DestroyGPUBuffer( pConstantBuffer->m_Handle );

        for ( auto pVertexShader : m_pVertexShaders )
            m_pDevice->DestroyShader( pVertexShader->m_Handle );

        for ( auto pPixelShader : m_pPixelShaders )
            m_pDevice->DestroyShader( pPixelShader->m_Handle );

        for ( auto pTexture : m_pTextures )
            m_pDevice->DestroyTexture( pTexture->m_Handle );

        for ( auto pSampler : m_pSamplers )
            m_pDevice->DestroySampler( pSampler->m_Handle );

        for ( auto pFramebuffer : m_pFramebuffers )
            m_pDevice->DestroyFramebuffer( pFramebuffer->m_Handle );

        for ( auto pBindingSet : m_pBindingSets )
            m_pDevice->DestroyBindingSet( pBindingSet->m_Handle );

        for ( auto pGraphicsPipeline : m_pGraphicsPipelines )
            m_pDevice->DestroyGraphicsPipeline( pGraphicsPipeline->m_Handle );
    }

    void ResourceManager::Initialize( rhi::GraphicsDevice *pDevice )
    {
        m_pDevice = pDevice;
    }

    VertexBuffer::Ref
    ResourceManager::CreateVertexBuffer( void *pVertices, const Count vertexCount, const rhi::BufferLayout &layout )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = rhi::BufferUsage::Immutable;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::VertexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pVertices );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout, vertexCount );
        m_pVertexBuffers.PushBack( pVertexBuffer );
        return pVertexBuffer;
    }

    VertexBuffer::Ref ResourceManager::CreateDynamicVertexBuffer( const Count vertexCount,
        const rhi::BufferLayout &layout )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = rhi::BufferUsage::Dynamic;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::VertexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout, vertexCount );
        m_pVertexBuffers.PushBack( pVertexBuffer );
        return pVertexBuffer;
    }

    memory::Ref< IndexBuffer > ResourceManager::CreateIndexBuffer( Uint32 *pIndices, const Count indexCount )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = indexCount * sizeof( Uint32 );
        bufferDesc.Usage = rhi::BufferUsage::Immutable;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::IndexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pIndices );

        auto pIndexBuffer = memory::CreateRef< IndexBuffer >( handle, indexCount );
        m_pIndexBuffers.PushBack( pIndexBuffer );
        return pIndexBuffer;
    }

    Texture::Ref ResourceManager::CreateTexture2D( Image::ConstRef pImage, bool updateable )
    {
        rhi::TextureDescriptor textureDesc{};
        textureDesc.Width = pImage->GetWidth();
        textureDesc.Height = pImage->GetHeight();
        textureDesc.TextureFormat = pImage->GetFormat();
        textureDesc.Dimension = rhi::TextureDimension::Texture2D;
        textureDesc.BindFlags = { rhi::TextureBindFlags::ShaderResource };
        textureDesc.CPUAccess = updateable ? rhi::CPUAccessMode::Write : rhi::CPUAccessMode::None;

        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        auto buffer = primitive::Vector< Byte >{ pImage->GetData(), pImage->GetDataSize() };
        m_pDevice->CreateTexture( handle, textureDesc, buffer );

        auto pTexture =
            memory::CreateRef< Texture >( handle, pImage->GetWidth(), pImage->GetHeight(), pImage->GetFormat() );
        m_pTextures.PushBack( pTexture );
        return pTexture;
    }

    Texture::Ref ResourceManager::CreateTextureCube( Image::ConstRef pImage, bool updateable )
    {
        rhi::TextureDescriptor textureDesc{};
        textureDesc.Width = pImage->GetWidth();
        textureDesc.Height = pImage->GetHeight();
        textureDesc.TextureFormat = pImage->GetFormat();
        textureDesc.Dimension = rhi::TextureDimension::TextureCube;
        textureDesc.BindFlags = { rhi::TextureBindFlags::ShaderResource };
        textureDesc.CPUAccess = updateable ? rhi::CPUAccessMode::Write : rhi::CPUAccessMode::Read;

        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        auto buffer = primitive::Vector< Byte >{ pImage->GetData(), pImage->GetDataSize() };
        m_pDevice->CreateTexture( handle, textureDesc, buffer );

        auto pTexture =
            memory::CreateRef< Texture >( handle, pImage->GetWidth(), pImage->GetHeight(), pImage->GetFormat() );
        m_pTextures.PushBack( pTexture );
        return pTexture;
    }

    Texture::Ref ResourceManager::CreateTextureFromNative( rhi::Object nativeTexture,
        rhi::ObjectType type,
        const rhi::TextureDescriptor &desc )
    {
        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        m_pDevice->CreateHandleForNativeTexture( handle, nativeTexture, type, desc );

        auto pTexture = memory::CreateRef< Texture >( handle, desc.Width, desc.Height, desc.TextureFormat );
        m_pTextures.PushBack( pTexture );
        return pTexture;
    }

    Sampler::Ref ResourceManager::CreateSampler( const rhi::SamplerDescriptor &descriptor )
    {
        rhi::SamplerHandle handle = m_SamplerHandleManager.CreateHandle();

        m_pDevice->CreateSampler( handle, descriptor );

        auto pSampler = memory::CreateRef< Sampler >( handle, descriptor );
        m_pSamplers.PushBack( pSampler );
        return pSampler;
    }

    Sampler::Ref ResourceManager::GetOrCreateSampler( const rhi::SamplerDescriptor &descriptor )
    {
        auto it = m_SamplerCache.FindItemAtKey( descriptor );

        if ( it != m_SamplerCache.end() )
        {
            return it.GetItem();
        }

        return CreateSampler( descriptor );
    }

    FramebufferAttachment ResourceManager::CreateColorAttachment( const Uint32 width, const Uint32 height )
    {
        rhi::TextureDescriptor textureDesc{};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.TextureFormat = rhi::Format::RGBA8_UNORM;
        textureDesc.Dimension = rhi::TextureDimension::Texture2D;
        textureDesc.BindFlags = { rhi::TextureBindFlags::RenderTarget };

        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        m_pDevice->CreateTexture( handle, textureDesc );

        auto pTexture = memory::CreateRef< Texture >( handle, width, height, textureDesc.TextureFormat );
        m_pTextures.PushBack( pTexture );

        return FramebufferAttachment{ pTexture, textureDesc.TextureFormat, true };
    }

    FramebufferAttachment ResourceManager::CreateDepthAttachment( const Uint32 width, const Uint32 height )
    {
        rhi::TextureDescriptor textureDesc{};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.TextureFormat = rhi::Format::D24S8;
        textureDesc.Dimension = rhi::TextureDimension::Texture2D;
        textureDesc.BindFlags = { rhi::TextureBindFlags::RenderTarget };

        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        m_pDevice->CreateTexture( handle, textureDesc );

        auto pTexture = memory::CreateRef< Texture >( handle, width, height, textureDesc.TextureFormat );
        m_pTextures.PushBack( pTexture );

        return FramebufferAttachment{ pTexture, textureDesc.TextureFormat, false };
    }

    ConstantBuffer::Ref ResourceManager::CreateConstantBuffer( const ConstantBufferDescriptor &descriptor )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = descriptor.GetSize();
        bufferDesc.Usage = rhi::BufferUsage::Dynamic;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::ConstantBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pConstantBuffer = memory::CreateRef< ConstantBuffer >( handle, descriptor );
        m_pConstantBuffers.PushBack( pConstantBuffer );
        return pConstantBuffer;
    }

    VertexShader::Ref ResourceManager::CreateVertexShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };

        if ( m_VertexShaderCache.HasItemAtKey( key ) )
        {
            SM_LOG_ERROR( "ResourceManager::CreateVertexShader > Vertex shader with the same bytecode already exists "
                          "in the cache" );
            return nullptr;
        }

        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Vertex };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        auto pVertexShader = memory::CreateRef< VertexShader >( handle );
        m_pVertexShaders.PushBack( pVertexShader );
        m_VertexShaderCache[key] = pVertexShader;

        return pVertexShader;
    }

    VertexShader::Ref ResourceManager::CreateVertexShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return CreateVertexShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    VertexShader::Ref ResourceManager::GetOrCreateVertexShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };
        auto it = m_VertexShaderCache.FindItemAtKey( key );

        if ( it != m_VertexShaderCache.end() )
        {
            return it.GetItem();
        }

        return CreateVertexShader( byteCode, entryPoint, targetProfile );
    }

    VertexShader::Ref ResourceManager::GetOrCreateVertexShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return GetOrCreateVertexShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    PixelShader::Ref ResourceManager::CreatePixelShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };

        if ( m_PixelShaderCache.HasItemAtKey( key ) )
        {
            SM_LOG_ERROR( "ResourceManager::CreatePixelShader > Pixel shader with the same bytecode already exists in "
                          "the cache" );
            return nullptr;
        }

        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Pixel };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        auto pPixelShader = memory::CreateRef< PixelShader >( handle );
        m_pPixelShaders.PushBack( pPixelShader );
        m_PixelShaderCache[key] = pPixelShader;

        return pPixelShader;
    }

    PixelShader::Ref ResourceManager::CreatePixelShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return CreatePixelShader( shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    PixelShader::Ref ResourceManager::GetOrCreatePixelShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };
        auto it = m_PixelShaderCache.FindItemAtKey( key );

        if ( it != m_PixelShaderCache.end() )
        {
            return it.GetItem();
        }

        return CreatePixelShader( byteCode, entryPoint, targetProfile );
    }

    PixelShader::Ref ResourceManager::GetOrCreatePixelShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return GetOrCreatePixelShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    Framebuffer::Ref ResourceManager::CreateFramebuffer(
        std::initializer_list< FramebufferAttachment > colorAttachments,
        const FramebufferAttachment &depthAttachment )
    {
        return CreateFramebuffer( primitive::Vector< FramebufferAttachment >{ colorAttachments }, depthAttachment );
    }

    Framebuffer::Ref ResourceManager::CreateFramebuffer(
        const primitive::Vector< FramebufferAttachment > &colorAttachments,
        const FramebufferAttachment &depthAttachment )
    {
        rhi::FramebufferDescriptor desc{};

        for ( const auto &attachment : colorAttachments )
        {
            rhi::TextureDescriptor colorDesc;
            colorDesc.TextureFormat = attachment.TextureFormat;
            colorDesc.CPUAccess = attachment.IsReadOnly ? rhi::CPUAccessMode::Read : rhi::CPUAccessMode::Write;
            colorDesc.Width = attachment.pTexture->GetWidth();
            colorDesc.Height = attachment.pTexture->GetHeight();

            desc.ColorAttachments.PushBack( rhi::FramebufferAttachment{ attachment.pTexture->GetHandle(), colorDesc } );
        }

        {
            rhi::TextureDescriptor depthDesc;
            depthDesc.TextureFormat = depthAttachment.TextureFormat;
            depthDesc.CPUAccess = depthAttachment.IsReadOnly ? rhi::CPUAccessMode::Read : rhi::CPUAccessMode::Write;
            depthDesc.Width = depthAttachment.pTexture->GetWidth();
            depthDesc.Height = depthAttachment.pTexture->GetHeight();

            desc.DepthAttachment = rhi::FramebufferAttachment{ depthAttachment.pTexture->GetHandle(), depthDesc };
        }

        rhi::FramebufferInfoExtented info{ desc }; // TODO: Maybe query device for this?

        rhi::FramebufferHandle handle = m_FramebufferHandleManager.CreateHandle();
        m_pDevice->CreateFramebuffer( handle, desc );

        auto pFramebuffer = memory::CreateRef< Framebuffer >( handle, colorAttachments, depthAttachment, info );
        m_pFramebuffers.PushBack( pFramebuffer );
        return pFramebuffer;
    }

    void ResourceManager::ResizeFramebuffer( Framebuffer::Ref pFramebuffer, const Uint32 width, const Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > rhi::s_MaxFramebufferSize ) ||
             ( height > rhi::s_MaxFramebufferSize ) )
        {
            SM_LOG_WARNING( "ResourceManager::ResizeFramebuffer > Invalid framebuffer size: {0}, {1}", width, height );
            return;
        }

        m_pDevice->DestroyFramebuffer( pFramebuffer->GetHandle() );

        primitive::Vector< FramebufferAttachment > newColorAttachments{};
        for ( auto &colorAttachment : pFramebuffer->GetColorAttachments() )
        {
            m_pDevice->DestroyTexture( colorAttachment.pTexture->GetHandle() );

            FramebufferAttachment newColorAttachment = CreateColorAttachment( width, height );
            newColorAttachments.EmplaceBack( std::move( newColorAttachment ) );
        }

        const FramebufferAttachment &depthAttachment = pFramebuffer->GetDepthAttachment();
        m_pDevice->DestroyTexture( depthAttachment.pTexture->GetHandle() );

        FramebufferAttachment newDepthAttachment = CreateDepthAttachment( width, height );

        pFramebuffer = CreateFramebuffer( newColorAttachments, newDepthAttachment );
    }

    BindingSet::Ref ResourceManager::CreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
        foundation::Flags< rhi::ShaderStage > shaderStage )
    {
        auto convertSetToLayout = []( const rhi::BindingSetDescriptor &desc, rhi::BindingLayout &layout )
        {
            for ( const rhi::BindingSetElement &element : desc )
            {
                const Uint16 size = [&]()
                {
                    if ( element.Type == rhi::ResourceType::PushConstants )
                        return element.Range.Size;
                    else
                        return 0u;
                }();

                rhi::BindingLayoutElement layoutElement{ element.Slot, element.Type, size };
                layout.AddElement( std::move( layoutElement ) );
            }
        };

        rhi::BindingLayout bindingLayout{ shaderStage };
        convertSetToLayout( descriptor, bindingLayout );

        rhi::BindingSetHandle handle = m_BindingSetHandleManager.CreateHandle();
        m_pDevice->CreateBindingSet( handle, descriptor, bindingLayout );

        auto pBindingSet = memory::CreateRef< BindingSet >( handle, descriptor, bindingLayout );
        m_pBindingSets.PushBack( pBindingSet );
        return pBindingSet;
    }

    GraphicsPipeline::Ref ResourceManager::CreateGraphicsPipeline( const GraphicsPipelineDescriptor &descriptor )
    {
        rhi::GraphicsPipelineDescriptor desc;
        desc.Topology = descriptor.Topology;
        desc.InputLayout = descriptor.InputLayout;
        desc.State = descriptor.RenderState;
        desc.VertexShaderHandle = descriptor.pVertexShader->m_Handle;
        desc.PixelShaderHandle = descriptor.pPixelShader->m_Handle;
        desc.BindingLayouts = descriptor.BindingLayouts;

        rhi::GraphicsPipelineHandle handle = m_GraphicsPipelineHandleManager.CreateHandle();
        m_pDevice->CreateGraphicsPipeline( handle, desc );

        auto pGraphicsPipeline = memory::CreateRef< GraphicsPipeline >( handle, descriptor );
        m_pGraphicsPipelines.PushBack( pGraphicsPipeline );
        return pGraphicsPipeline;
    }

    rhi::Object ResourceManager::GetShaderResourceView( Texture::ConstRef pTexture )
    {
        return m_pDevice->GetNativeView( pTexture->GetHandle(),
            rhi::ObjectType::D3D11_ShaderResourceView,
            pTexture->GetFormat(),
            rhi::TextureSubresourceSet{},
            rhi::TextureDimension::Texture2D );
    }
}