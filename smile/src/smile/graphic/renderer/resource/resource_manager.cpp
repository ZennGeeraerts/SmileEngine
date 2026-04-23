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
        for ( const auto &vertexBuffer : m_VertexBuffers )
            m_pDevice->DestroyGPUBuffer( vertexBuffer.m_Handle );

        for ( const auto &indexBuffer : m_IndexBuffers )
            m_pDevice->DestroyGPUBuffer( indexBuffer.m_Handle );

        for ( const auto &constantBuffer : m_ConstantBuffers )
            m_pDevice->DestroyGPUBuffer( constantBuffer.m_Handle );

        for ( const auto &vertexShader : m_VertexShaders )
            m_pDevice->DestroyShader( vertexShader.m_Handle );

        for ( const auto &pixelShader : m_PixelShaders )
            m_pDevice->DestroyShader( pixelShader.m_Handle );

        for ( const auto &texture : m_Textures )
            m_pDevice->DestroyTexture( texture.m_Handle );

        for ( const auto &sampler : m_Samplers )
            m_pDevice->DestroySampler( sampler.m_Handle );

        for ( const auto &framebuffer : m_Framebuffers )
            m_pDevice->DestroyFramebuffer( framebuffer.m_Handle );

        for ( const auto &bindingSet : m_BindingSets )
            m_pDevice->DestroyBindingSet( bindingSet.m_Handle );

        for ( const auto &graphicsPipeline : m_GraphicsPipelines )
            m_pDevice->DestroyGraphicsPipeline( graphicsPipeline.m_Handle );
    }

    void ResourceManager::Initialize( rhi::GraphicsDevice *pDevice )
    {
        m_pDevice = pDevice;
    }

    VertexBuffer
    ResourceManager::CreateVertexBuffer( void *pVertices, const Count vertexCount, const rhi::BufferLayout &layout )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = rhi::BufferUsage::Immutable;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::VertexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pVertices );

        const VertexBuffer vertexBuffer{ handle, layout, vertexCount };
        m_VertexBuffers.PushBack( vertexBuffer );
        return vertexBuffer;
    }

    VertexBuffer ResourceManager::CreateDynamicVertexBuffer( const Count vertexCount, const rhi::BufferLayout &layout )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = rhi::BufferUsage::Dynamic;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::VertexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        const VertexBuffer vertexBuffer{ handle, layout, vertexCount };
        m_VertexBuffers.PushBack( vertexBuffer );
        return vertexBuffer;
    }

    IndexBuffer ResourceManager::CreateIndexBuffer( Uint32 *pIndices, const Count indexCount )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = indexCount * sizeof( Uint32 );
        bufferDesc.Usage = rhi::BufferUsage::Immutable;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::IndexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pIndices );

        const IndexBuffer indexBuffer{ handle, indexCount };
        m_IndexBuffers.PushBack( indexBuffer );
        return indexBuffer;
    }

    Texture ResourceManager::CreateTexture2D( Image::ConstRef pImage, bool updateable )
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

        const Texture texture{ handle, pImage->GetWidth(), pImage->GetHeight(), pImage->GetFormat() };
        m_Textures.PushBack( texture );
        return texture;
    }

    Texture ResourceManager::CreateTextureCube( Image::ConstRef pImage, bool updateable )
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

        const Texture texture{ handle, pImage->GetWidth(), pImage->GetHeight(), pImage->GetFormat() };
        m_Textures.PushBack( texture );
        return texture;
    }

    Texture ResourceManager::CreateTextureFromNative( rhi::Object nativeTexture,
        rhi::ObjectType type,
        const rhi::TextureDescriptor &desc )
    {
        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        m_pDevice->CreateHandleForNativeTexture( handle, nativeTexture, type, desc );

        const Texture texture{ handle, desc.Width, desc.Height, desc.TextureFormat };
        m_Textures.PushBack( texture );
        return texture;
    }

    Sampler ResourceManager::CreateSampler( const rhi::SamplerDescriptor &descriptor )
    {
        rhi::SamplerHandle handle = m_SamplerHandleManager.CreateHandle();

        m_pDevice->CreateSampler( handle, descriptor );

        const Sampler sampler{ handle, descriptor };
        m_Samplers.PushBack( sampler );
        return sampler;
    }

    Sampler ResourceManager::GetOrCreateSampler( const rhi::SamplerDescriptor &descriptor )
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

        const Texture texture{ handle, width, height, textureDesc.TextureFormat };
        m_Textures.PushBack( texture );

        return FramebufferAttachment{ texture, textureDesc.TextureFormat, true };
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

        const Texture texture{ handle, width, height, textureDesc.TextureFormat };
        m_Textures.PushBack( texture );

        return FramebufferAttachment{ texture, textureDesc.TextureFormat, false };
    }

    ConstantBuffer ResourceManager::CreateConstantBuffer( const ConstantBufferDescriptor &descriptor )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = descriptor.GetSize();
        bufferDesc.Usage = rhi::BufferUsage::Dynamic;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::ConstantBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        const ConstantBuffer constantBuffer{ handle, descriptor };
        m_ConstantBuffers.PushBack( constantBuffer );
        return constantBuffer;
    }

    VertexShader ResourceManager::CreateVertexShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };

        if ( m_VertexShaderCache.HasItemAtKey( key ) )
        {
            SM_LOG_ERROR( "ResourceManager::CreateVertexShader > Vertex shader with the same bytecode already exists "
                          "in the cache" );
            return {};
        }

        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Vertex };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        const VertexShader vertexShader{ handle };
        m_VertexShaders.PushBack( vertexShader );
        m_VertexShaderCache[key] = vertexShader;

        return vertexShader;
    }

    VertexShader ResourceManager::CreateVertexShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return CreateVertexShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    VertexShader ResourceManager::GetOrCreateVertexShader( const primitive::Vector< Byte > &byteCode,
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

    VertexShader ResourceManager::GetOrCreateVertexShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return GetOrCreateVertexShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    PixelShader ResourceManager::CreatePixelShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };

        if ( m_PixelShaderCache.HasItemAtKey( key ) )
        {
            SM_LOG_ERROR( "ResourceManager::CreatePixelShader > Pixel shader with the same bytecode already exists in "
                          "the cache" );
            return {};
        }

        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Pixel };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        const PixelShader pixelShader{ handle };
        m_PixelShaders.PushBack( pixelShader );
        m_PixelShaderCache[key] = pixelShader;

        return pixelShader;
    }

    PixelShader ResourceManager::CreatePixelShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return CreatePixelShader( shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    PixelShader ResourceManager::GetOrCreatePixelShader( const primitive::Vector< Byte > &byteCode,
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

    PixelShader ResourceManager::GetOrCreatePixelShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return GetOrCreatePixelShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    Framebuffer ResourceManager::CreateFramebuffer( std::initializer_list< FramebufferAttachment > colorAttachments,
        const FramebufferAttachment &depthAttachment )
    {
        return CreateFramebuffer( primitive::Vector< FramebufferAttachment >{ colorAttachments }, depthAttachment );
    }

    Framebuffer ResourceManager::CreateFramebuffer( const primitive::Vector< FramebufferAttachment > &colorAttachments,
        const FramebufferAttachment &depthAttachment )
    {
        rhi::FramebufferDescriptor desc{};

        for ( const auto &attachment : colorAttachments )
        {
            rhi::TextureDescriptor colorDesc;
            colorDesc.TextureFormat = attachment.TextureFormat;
            colorDesc.CPUAccess = attachment.IsReadOnly ? rhi::CPUAccessMode::Read : rhi::CPUAccessMode::Write;
            colorDesc.Width = attachment.Texture.GetWidth();
            colorDesc.Height = attachment.Texture.GetHeight();

            desc.ColorAttachments.PushBack( rhi::FramebufferAttachment{ attachment.Texture.GetHandle(), colorDesc } );
        }

        {
            rhi::TextureDescriptor depthDesc;
            depthDesc.TextureFormat = depthAttachment.TextureFormat;
            depthDesc.CPUAccess = depthAttachment.IsReadOnly ? rhi::CPUAccessMode::Read : rhi::CPUAccessMode::Write;
            depthDesc.Width = depthAttachment.Texture.GetWidth();
            depthDesc.Height = depthAttachment.Texture.GetHeight();

            desc.DepthAttachment = rhi::FramebufferAttachment{ depthAttachment.Texture.GetHandle(), depthDesc };
        }

        rhi::FramebufferInfoExtented info{ desc }; // TODO: Maybe query device for this?

        rhi::FramebufferHandle handle = m_FramebufferHandleManager.CreateHandle();
        m_pDevice->CreateFramebuffer( handle, desc );

        const Framebuffer framebuffer{ handle, colorAttachments, depthAttachment, info };
        m_Framebuffers.PushBack( framebuffer );
        return framebuffer;
    }

    void ResourceManager::ResizeFramebuffer( Framebuffer &framebuffer, const Uint32 width, const Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > rhi::s_MaxFramebufferSize ) ||
             ( height > rhi::s_MaxFramebufferSize ) )
        {
            SM_LOG_WARNING( "ResourceManager::ResizeFramebuffer > Invalid framebuffer size: {0}, {1}", width, height );
            return;
        }

        m_pDevice->DestroyFramebuffer( framebuffer.GetHandle() );

        primitive::Vector< FramebufferAttachment > newColorAttachments{};
        for ( auto &colorAttachment : framebuffer.GetColorAttachments() )
        {
            m_pDevice->DestroyTexture( colorAttachment.Texture.GetHandle() );

            FramebufferAttachment newColorAttachment = CreateColorAttachment( width, height );
            newColorAttachments.EmplaceBack( std::move( newColorAttachment ) );
        }

        const FramebufferAttachment &depthAttachment = framebuffer.GetDepthAttachment();
        m_pDevice->DestroyTexture( depthAttachment.Texture.GetHandle() );

        FramebufferAttachment newDepthAttachment = CreateDepthAttachment( width, height );

        framebuffer = CreateFramebuffer( newColorAttachments, newDepthAttachment );
    }

    BindingSet ResourceManager::CreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
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

        const BindingSet bindingSet{ handle, descriptor, bindingLayout };
        m_BindingSets.PushBack( bindingSet );
        return bindingSet;
    }

    GraphicsPipeline ResourceManager::CreateGraphicsPipeline( const GraphicsPipelineDescriptor &descriptor )
    {
        rhi::GraphicsPipelineDescriptor desc;
        desc.Topology = descriptor.Topology;
        desc.InputLayout = descriptor.InputLayout;
        desc.State = descriptor.RenderState;
        desc.VertexShaderHandle = descriptor.VertexShader.GetHandle();
        desc.PixelShaderHandle = descriptor.PixelShader.GetHandle();
        desc.BindingLayouts = descriptor.BindingLayouts;

        rhi::GraphicsPipelineHandle handle = m_GraphicsPipelineHandleManager.CreateHandle();
        m_pDevice->CreateGraphicsPipeline( handle, desc );

        const GraphicsPipeline pipeline{ handle, descriptor };
        m_GraphicsPipelines.PushBack( pipeline );
        return pipeline;
    }

    rhi::Object ResourceManager::GetShaderResourceView( const Texture &texture )
    {
        return m_pDevice->GetNativeView( texture.GetHandle(),
            rhi::ObjectType::D3D11_ShaderResourceView,
            texture.GetFormat(),
            rhi::TextureSubresourceSet{},
            rhi::TextureDimension::Texture2D );
    }
}