/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "resource_manager.h"

#include "smile/graphic/rhi/graphics_device.h"

namespace smile::graphic
{
    namespace detail
    {
        static void ConvertBindingSetToLayout( const rhi::BindingSetDescriptor &desc, rhi::BindingLayout &layout )
        {
            for ( const rhi::BindingSetElement &element : desc )
            {
                const Uint16 size = [&element]()
                {
                    if ( element.Type == rhi::ResourceType::PushConstants )
                        return element.Range.Size;
                    else
                        return 0u;
                }();

                rhi::BindingLayoutElement layoutElement{ element.Slot, element.Type, size };
                layout.AddElement( std::move( layoutElement ) );
            }
        }
    }

    ResourceManager::ResourceManager( rhi::GraphicsDevice &device ) noexcept : m_Device{ device }
    {
    }

    ResourceManager::~ResourceManager()
    {
        for ( const auto &vertexBuffer : m_VertexBuffers )
            m_Device.DestroyGPUBuffer( vertexBuffer.GetHandle() );

        for ( const auto &indexBuffer : m_IndexBuffers )
            m_Device.DestroyGPUBuffer( indexBuffer.GetHandle() );

        for ( const auto &constantBuffer : m_ConstantBuffers )
            m_Device.DestroyGPUBuffer( constantBuffer.GetHandle() );

        for ( const auto &vertexShader : m_VertexShaders )
            m_Device.DestroyShader( vertexShader.GetHandle() );

        for ( const auto &pixelShader : m_PixelShaders )
            m_Device.DestroyShader( pixelShader.GetHandle() );

        for ( const auto &texture : m_Textures )
            m_Device.DestroyTexture( texture.GetHandle() );

        for ( const auto &sampler : m_Samplers )
            m_Device.DestroySampler( sampler.GetHandle() );

        for ( const auto &framebuffer : m_Framebuffers )
            m_Device.DestroyFramebuffer( framebuffer.GetHandle() );

        for ( const auto &bindingLayout : m_BindingLayouts )
            m_Device.DestroyBindingLayout( bindingLayout.GetHandle() );

        for ( const auto &bindingSet : m_BindingSets )
            m_Device.DestroyBindingSet( bindingSet.GetHandle() );

        for ( const auto &graphicsPipeline : m_GraphicsPipelines )
            m_Device.DestroyGraphicsPipeline( graphicsPipeline.GetHandle() );
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
        m_Device.CreateGPUBuffer( handle, bufferDesc, pVertices );

        const VertexBuffer vertexBuffer{ handle, vertexCount, layout.GetStride() };
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
        m_Device.CreateGPUBuffer( handle, bufferDesc );

        const VertexBuffer vertexBuffer{ handle, vertexCount, layout.GetStride() };
        m_VertexBuffers.PushBack( vertexBuffer );
        return vertexBuffer;
    }

    void ResourceManager::DestroyVertexBuffer( VertexBuffer &vertexBuffer )
    {
        m_Device.DestroyGPUBuffer( vertexBuffer.GetHandle() );
        m_VertexBuffers.Erase( vertexBuffer );
        m_GPUBufferHandleManager.DestroyHandle( vertexBuffer.GetHandle() );
        vertexBuffer.m_Handle = rhi::GPUBufferHandle::NullHandle();
    }

    IndexBuffer ResourceManager::CreateIndexBuffer( Uint32 *pIndices, const Count indexCount )
    {
        rhi::GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = indexCount * sizeof( Uint32 );
        bufferDesc.Usage = rhi::BufferUsage::Immutable;
        bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
        bufferDesc.BindFlags = { rhi::BufferBindFlags::IndexBuffer };

        rhi::GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_Device.CreateGPUBuffer( handle, bufferDesc, pIndices );

        const IndexBuffer indexBuffer{ handle, indexCount };
        m_IndexBuffers.PushBack( indexBuffer );
        return indexBuffer;
    }

    void ResourceManager::DestroyIndexBuffer( IndexBuffer &indexBuffer )
    {
        m_Device.DestroyGPUBuffer( indexBuffer.GetHandle() );
        m_IndexBuffers.Erase( indexBuffer );
        m_GPUBufferHandleManager.DestroyHandle( indexBuffer.GetHandle() );
        indexBuffer.m_Handle = rhi::GPUBufferHandle::NullHandle();
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
        m_Device.CreateTexture( handle, textureDesc, buffer );

        const Texture texture{ handle, pImage->GetWidth(), pImage->GetHeight(), pImage->GetFormat() };
        m_Textures.PushBack( texture );
        return texture;
    }

    Texture ResourceManager::CreateTexture2D( TextureAsset::ConstRef textureAsset )
    {
        const auto texture = CreateTexture2D( textureAsset->GetImage(), false );
        m_TextureCache.Add( textureAsset, texture );
        return texture;
    }

    Texture ResourceManager::GetOrCreateTexture2D( TextureAsset::ConstRef textureAsset )
    {
        const auto texture = m_TextureCache.Find( textureAsset );

        if ( texture.has_value() )
        {
            return texture.value();
        }

        return CreateTexture2D( textureAsset );
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
        m_Device.CreateTexture( handle, textureDesc, buffer );

        const Texture texture{ handle, pImage->GetWidth(), pImage->GetHeight(), pImage->GetFormat() };
        m_Textures.PushBack( texture );
        return texture;
    }

    Texture ResourceManager::CreateTextureFromNative( rhi::Object nativeTexture,
        rhi::ObjectType type,
        const rhi::TextureDescriptor &desc )
    {
        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        m_Device.CreateHandleForNativeTexture( handle, nativeTexture, type, desc );

        const Texture texture{ handle, desc.Width, desc.Height, desc.TextureFormat };
        m_Textures.PushBack( texture );
        return texture;
    }

    void ResourceManager::DestroyTexture( Texture &texture )
    {
        m_Device.DestroyTexture( texture.GetHandle() );
        m_Textures.Erase( texture );
        m_TextureCache.Invalidate( texture );

        m_TextureHandleManager.DestroyHandle( texture.GetHandle() );
        texture.m_Handle = rhi::TextureHandle::NullHandle();
    }

    Sampler ResourceManager::CreateSampler( const rhi::SamplerDescriptor &descriptor )
    {
        rhi::SamplerHandle handle = m_SamplerHandleManager.CreateHandle();

        m_Device.CreateSampler( handle, descriptor );

        const Sampler sampler{ handle };
        m_Samplers.PushBack( sampler );
        m_SamplerCache.Add( descriptor, sampler );
        return sampler;
    }

    Sampler ResourceManager::GetOrCreateSampler( const rhi::SamplerDescriptor &descriptor )
    {
        const auto sampler = m_SamplerCache.Find( descriptor );

        if ( sampler.has_value() )
        {
            return sampler.value();
        }

        return CreateSampler( descriptor );
    }

    void ResourceManager::DestroySampler( Sampler &sampler )
    {
        m_Device.DestroySampler( sampler.GetHandle() );
        m_Samplers.Erase( sampler );
        m_SamplerCache.Invalidate( sampler );

        m_SamplerHandleManager.DestroyHandle( sampler.GetHandle() );
        sampler.m_Handle = rhi::SamplerHandle::NullHandle();
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

        m_Device.CreateTexture( handle, textureDesc );

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

        m_Device.CreateTexture( handle, textureDesc );

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
        m_Device.CreateGPUBuffer( handle, bufferDesc );

        const ConstantBuffer constantBuffer{ handle, bufferDesc.Size };
        m_ConstantBuffers.PushBack( constantBuffer );
        m_ConstantBufferCache.Add( descriptor, constantBuffer );

        return constantBuffer;
    }

    ConstantBuffer ResourceManager::GetOrCreateConstantBuffer( const ConstantBufferDescriptor &descriptor )
    {
        const auto constantBuffer = m_ConstantBufferCache.Find( descriptor );

        if ( constantBuffer )
        {
            return *constantBuffer;
        }

        return CreateConstantBuffer( descriptor );
    }

    void ResourceManager::DestroyConstantBuffer( ConstantBuffer &constantBuffer )
    {
        m_Device.DestroyGPUBuffer( constantBuffer.GetHandle() );
        m_ConstantBuffers.Erase( constantBuffer );
        m_ConstantBufferCache.Invalidate( constantBuffer );

        m_GPUBufferHandleManager.DestroyHandle( constantBuffer.GetHandle() );
        constantBuffer.m_Handle = rhi::GPUBufferHandle::NullHandle();
    }

    VertexShader ResourceManager::CreateVertexShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };

        if ( m_VertexShaderCache.Has( key ) )
        {
            SM_LOG_ERROR( "ResourceManager::CreateVertexShader > Vertex shader with the same bytecode already exists "
                          "in the cache" );
            return {};
        }

        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Vertex };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_Device.CreateShader( handle, shaderDesc, byteCode );

        const VertexShader vertexShader{ handle };
        m_VertexShaders.PushBack( vertexShader );
        m_VertexShaderCache.Add( key, vertexShader );

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
        const auto vertexShader = m_VertexShaderCache.Find( key );

        if ( vertexShader.has_value() )
        {
            return vertexShader.value();
        }

        return CreateVertexShader( byteCode, entryPoint, targetProfile );
    }

    VertexShader ResourceManager::GetOrCreateVertexShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return GetOrCreateVertexShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    void ResourceManager::DestroyVertexShader( VertexShader &vertexShader )
    {
        m_Device.DestroyShader( vertexShader.GetHandle() );
        m_VertexShaders.Erase( vertexShader );
        m_VertexShaderCache.Invalidate( vertexShader );

        m_ShaderHandleManager.DestroyHandle( vertexShader.GetHandle() );
        vertexShader.m_Handle = rhi::ShaderHandle::NullHandle();
    }

    PixelShader ResourceManager::CreatePixelShader( const primitive::Vector< Byte > &byteCode,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        const ShaderKey key{ byteCode, entryPoint, targetProfile };

        if ( m_PixelShaderCache.Has( key ) )
        {
            SM_LOG_ERROR( "ResourceManager::CreatePixelShader > Pixel shader with the same bytecode already exists in "
                          "the cache" );
            return {};
        }

        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Pixel };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_Device.CreateShader( handle, shaderDesc, byteCode );

        const PixelShader pixelShader{ handle };
        m_PixelShaders.PushBack( pixelShader );
        m_PixelShaderCache.Add( key, pixelShader );

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
        const auto pixelShader = m_PixelShaderCache.Find( key );

        if ( pixelShader.has_value() )
        {
            return pixelShader.value();
        }

        return CreatePixelShader( byteCode, entryPoint, targetProfile );
    }

    PixelShader ResourceManager::GetOrCreatePixelShader( ShaderAsset::ConstRef shaderAsset )
    {
        const auto &reflectionData = shaderAsset->GetReflectionData();

        return GetOrCreatePixelShader(
            shaderAsset->GetByteCode(), reflectionData.EntryPoint, reflectionData.TargetProfile );
    }

    void ResourceManager::DestroyPixelShader( PixelShader &pixelShader )
    {
        m_Device.DestroyShader( pixelShader.GetHandle() );
        m_PixelShaders.Erase( pixelShader );
        m_PixelShaderCache.Invalidate( pixelShader );

        m_ShaderHandleManager.DestroyHandle( pixelShader.GetHandle() );
        pixelShader.m_Handle = rhi::ShaderHandle::NullHandle();
    }

    Framebuffer ResourceManager::CreateFramebuffer( std::initializer_list< FramebufferAttachment > colorAttachments,
        const FramebufferAttachment &depthAttachment )
    {
        return CreateFramebuffer(
            primitive::FixedVector< FramebufferAttachment, rhi::s_MaxRenderTargets >{ colorAttachments },
            depthAttachment );
    }

    Framebuffer ResourceManager::CreateFramebuffer(
        const primitive::FixedVector< FramebufferAttachment, rhi::s_MaxRenderTargets > &colorAttachments,
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

        rhi::FramebufferInfoExtented info{ desc };

        rhi::FramebufferHandle handle = m_FramebufferHandleManager.CreateHandle();
        m_Device.CreateFramebuffer( handle, desc );

        const FramebufferAttachmentSetHandle attachmentSetHandle =
            m_FramebufferAttachmentSetHandleManager.CreateHandle();

        FramebufferAttachmentSet attachmentSet{ colorAttachments, depthAttachment };
        m_FramebufferAttachmentSets[attachmentSetHandle.GetIndex()] = std::move( attachmentSet );

        const Framebuffer framebuffer{ handle, attachmentSetHandle, info.Width, info.Height };
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

        DestroyFramebuffer( framebuffer );

        FramebufferAttachmentSet &attachmentSet = GetFramebufferAttachmentSet( framebuffer );
        primitive::FixedVector< FramebufferAttachment, rhi::s_MaxRenderTargets > newColorAttachments{};

        for ( auto &colorAttachment : attachmentSet.ColorAttachments )
        {
            if ( colorAttachment.Texture.IsValid() )
                DestroyTexture( colorAttachment.Texture );

            FramebufferAttachment newColorAttachment = CreateColorAttachment( width, height );
            newColorAttachments.PushBack( std::move( newColorAttachment ) );
        }

        if ( attachmentSet.DepthAttachment.Texture.IsValid() )
            DestroyTexture( attachmentSet.DepthAttachment.Texture );

        FramebufferAttachment newDepthAttachment = CreateDepthAttachment( width, height );

        m_FramebufferAttachmentSetHandleManager.DestroyHandle( framebuffer.GetAttachmentSetHandle() );
        framebuffer.m_AttachmentSetHandle = FramebufferAttachmentSetHandle::NullHandle();

        framebuffer = CreateFramebuffer( newColorAttachments, newDepthAttachment );
    }

    void ResourceManager::DestroyFramebuffer( Framebuffer &framebuffer, bool destroyTextureAttachments )
    {
        m_Device.DestroyFramebuffer( framebuffer.GetHandle() );
        m_Framebuffers.Erase( framebuffer );

        if ( destroyTextureAttachments )
        {
            FramebufferAttachmentSet &attachmentSet = GetFramebufferAttachmentSet( framebuffer );
            for ( auto &colorAttachment : attachmentSet.ColorAttachments )
            {
                if ( colorAttachment.Texture.IsValid() )
                    DestroyTexture( colorAttachment.Texture );
            }

            if ( attachmentSet.DepthAttachment.Texture.IsValid() )
                DestroyTexture( attachmentSet.DepthAttachment.Texture );
        }

        m_FramebufferAttachmentSetHandleManager.DestroyHandle( framebuffer.GetAttachmentSetHandle() );
        framebuffer.m_AttachmentSetHandle = FramebufferAttachmentSetHandle::NullHandle();

        m_FramebufferHandleManager.DestroyHandle( framebuffer.GetHandle() );
        framebuffer.m_Handle = rhi::FramebufferHandle::NullHandle();
    }

    BindingLayout ResourceManager::CreateBindingLayout( const rhi::BindingLayout &layout )
    {
        rhi::BindingLayoutHandle handle = m_BindingLayoutHandleManager.CreateHandle();
        m_Device.CreateBindingLayout( handle, layout );

        const BindingLayout bindingLayout{ handle };
        m_BindingLayouts.PushBack( bindingLayout );
        m_BindingLayoutCache.Add( layout, bindingLayout );

        return bindingLayout;
    }

    BindingLayout ResourceManager::GetOrCreateBindingLayout( const rhi::BindingLayout &layout )
    {
        const auto bindingLayout = m_BindingLayoutCache.Find( layout );

        if ( bindingLayout.has_value() )
        {
            return bindingLayout.value();
        }

        return CreateBindingLayout( layout );
    }

    void ResourceManager::DestroyBindingLayout( BindingLayout &bindingLayout )
    {
        m_Device.DestroyBindingLayout( bindingLayout.GetHandle() );
        m_BindingLayouts.Erase( bindingLayout );

        m_BindingLayoutCache.Invalidate( bindingLayout );

        m_BindingLayoutHandleManager.DestroyHandle( bindingLayout.GetHandle() );
        bindingLayout.m_Handle = rhi::BindingLayoutHandle::NullHandle();
    }

    BindingSet ResourceManager::CreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
        const BindingLayout &layout,
        foundation::Flags< rhi::ShaderStage > shaderStage )
    {
        rhi::BindingSetHandle handle = m_BindingSetHandleManager.CreateHandle();
        m_Device.CreateBindingSet( handle, descriptor, layout.GetHandle() );

        const BindingSet bindingSet{ handle };
        m_BindingSets.PushBack( bindingSet );
        m_BindingSetCache.Add( descriptor, bindingSet );

        return bindingSet;
    }

    BindingSet ResourceManager::GetOrCreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
        const BindingLayout &layout,
        foundation::Flags< rhi::ShaderStage > shaderStage )
    {
        const auto bindingSet = m_BindingSetCache.Find( descriptor );

        if ( bindingSet.has_value() )
        {
            return bindingSet.value();
        }

        return CreateBindingSet( descriptor, layout, shaderStage );
    }

    void ResourceManager::DestroyBindingSet( BindingSet &bindingSet )
    {
        m_Device.DestroyBindingSet( bindingSet.GetHandle() );
        m_BindingSets.Erase( bindingSet );
        m_BindingSetCache.Invalidate( bindingSet );

        m_BindingSetHandleManager.DestroyHandle( bindingSet.GetHandle() );
        bindingSet.m_Handle = rhi::BindingSetHandle::NullHandle();
    }

    void ResourceManager::CreateBindingSetAndLayout( const rhi::BindingSetDescriptor &descriptor,
        foundation::Flags< rhi::ShaderStage > shaderStage,
        BindingLayout &layout,
        BindingSet &set )
    {
        rhi::BindingLayout bindingLayout{ shaderStage };
        detail::ConvertBindingSetToLayout( descriptor, bindingLayout );

        layout = CreateBindingLayout( bindingLayout );
        set = CreateBindingSet( descriptor, layout, shaderStage );
    }

    void ResourceManager::GetOrCreateBindingSetAndLayout( const rhi::BindingSetDescriptor &descriptor,
        foundation::Flags< rhi::ShaderStage > shaderStage,
        BindingLayout &layout,
        BindingSet &set )
    {
        rhi::BindingLayout bindingLayout{ shaderStage };
        detail::ConvertBindingSetToLayout( descriptor, bindingLayout );

        layout = GetOrCreateBindingLayout( bindingLayout );
        set = GetOrCreateBindingSet( descriptor, layout, shaderStage );
    }

    GraphicsPipeline ResourceManager::CreateGraphicsPipeline( const GraphicsPipelineDescriptor &descriptor )
    {
        rhi::GraphicsPipelineDescriptor desc;
        desc.Topology = descriptor.Topology;
        desc.InputLayout = descriptor.InputLayout;
        desc.State = descriptor.RenderState;
        desc.VertexShaderHandle = descriptor.VertexShader.GetHandle();
        desc.PixelShaderHandle = descriptor.PixelShader.GetHandle();

        for ( const auto &bindingLayout : descriptor.BindingLayouts )
        {
            desc.BindingLayouts.PushBack( bindingLayout.GetHandle() );
        }

        rhi::GraphicsPipelineHandle handle = m_GraphicsPipelineHandleManager.CreateHandle();
        m_Device.CreateGraphicsPipeline( handle, desc );

        const GraphicsPipeline pipeline{ handle };
        m_GraphicsPipelines.PushBack( pipeline );
        m_GraphicsPipelineCache.Add( descriptor, pipeline );
        return pipeline;
    }

    GraphicsPipeline ResourceManager::GetOrCreateGraphicsPipeline( const GraphicsPipelineDescriptor &descriptor )
    {
        const auto pipeline = m_GraphicsPipelineCache.Find( descriptor );

        if ( pipeline.has_value() )
        {
            return pipeline.value();
        }

        return CreateGraphicsPipeline( descriptor );
    }

    void ResourceManager::DestroyGraphicsPipeline( GraphicsPipeline &pipeline )
    {
        m_Device.DestroyGraphicsPipeline( pipeline.GetHandle() );
        m_GraphicsPipelines.Erase( pipeline );
        m_GraphicsPipelineCache.Invalidate( pipeline );

        m_GraphicsPipelineHandleManager.DestroyHandle( pipeline.GetHandle() );
        pipeline.m_Handle = rhi::GraphicsPipelineHandle::NullHandle();
    }

    rhi::Object ResourceManager::GetShaderResourceView( const Texture &texture )
    {
        return m_Device.GetNativeView( texture.GetHandle(),
            rhi::ObjectType::D3D11_ShaderResourceView,
            texture.GetFormat(),
            rhi::TextureSubresourceSet{},
            rhi::TextureDimension::Texture2D );
    }

    FramebufferAttachmentSet &ResourceManager::GetFramebufferAttachmentSet( const Framebuffer &framebuffer )
    {
        SM_ASSERT( framebuffer.GetAttachmentSetHandle().IsValid() )

        return m_FramebufferAttachmentSets[framebuffer.GetAttachmentSetHandle().GetIndex()];
    }

    const FramebufferAttachmentSet &ResourceManager::GetFramebufferAttachmentSet( const Framebuffer &framebuffer ) const
    {
        SM_ASSERT( framebuffer.GetAttachmentSetHandle().IsValid() )

        return m_FramebufferAttachmentSets[framebuffer.GetAttachmentSetHandle().GetIndex()];
    }
}