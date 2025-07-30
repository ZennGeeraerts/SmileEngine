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

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout );
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
        textureDesc.CPUAccess = updateable ? rhi::CPUAccessMode::Write : rhi::CPUAccessMode::Read;

        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        auto buffer = std::vector< Byte >{ pImage->GetData(), pImage->GetData() + pImage->GetDataSize() };
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

        auto buffer = std::vector< Byte >{ pImage->GetData(), pImage->GetData() + pImage->GetDataSize() };
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

    FramebufferAttachment ResourceManager::CreateColorAttachment( const Uint32 width, const Uint32 height )
    {
        rhi::TextureDescriptor textureDesc{};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.TextureFormat = rhi::Format::RGBA8_UNORM;
        textureDesc.Dimension = rhi::TextureDimension::Texture2D;
        textureDesc.BindFlags = { rhi::TextureBindFlags::RenderTarget };
        textureDesc.CPUAccess = rhi::CPUAccessMode::Read;

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
        textureDesc.CPUAccess = rhi::CPUAccessMode::Read;

        rhi::TextureHandle handle = m_TextureHandleManager.CreateHandle();

        m_pDevice->CreateTexture( handle, textureDesc );

        auto pTexture = memory::CreateRef< Texture >( handle, width, height, textureDesc.TextureFormat );
        m_pTextures.PushBack( pTexture );

        return FramebufferAttachment{ pTexture, textureDesc.TextureFormat, true };
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

    VertexShader::Ref ResourceManager::CreateVertexShader( const std::vector< Byte > &byteCode,
        const std::string &entryPoint,
        const std::string &targetProfile )
    {
        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Vertex };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        auto pVertexShader = memory::CreateRef< VertexShader >( handle );
        m_pVertexShaders.PushBack( pVertexShader );
        return pVertexShader;
    }

    PixelShader::Ref ResourceManager::CreatePixelShader( const std::vector< Byte > &byteCode,
        const std::string &entryPoint,
        const std::string &targetProfile )
    {
        rhi::ShaderDescriptor shaderDesc{ rhi::ShaderStage::Pixel };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        rhi::ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        auto pPixelShader = memory::CreateRef< PixelShader >( handle );
        m_pPixelShaders.PushBack( pPixelShader );
        return pPixelShader;
    }

    Framebuffer::Ref ResourceManager::CreateFramebuffer(
        std::initializer_list< FramebufferAttachment > colorAttachments,
        const FramebufferAttachment &depthAttachment )
    {
        rhi::FramebufferDescriptor desc{};

        for ( const auto &attachment : colorAttachments )
        {
            rhi::TextureDescriptor colorDesc;
            colorDesc.TextureFormat = attachment.TextureFormat;
            colorDesc.CPUAccess = attachment.IsReadOnly ? rhi::CPUAccessMode::Read : rhi::CPUAccessMode::Write;

            desc.ColorAttachments.PushBack( rhi::FramebufferAttachment{ attachment.pTexture->GetHandle(), colorDesc } );
        }

        {
            rhi::TextureDescriptor depthDesc;
            depthDesc.TextureFormat = depthAttachment.TextureFormat;
            depthDesc.CPUAccess = depthAttachment.IsReadOnly ? rhi::CPUAccessMode::Read : rhi::CPUAccessMode::Write;

            desc.DepthAttachment = rhi::FramebufferAttachment{ depthAttachment.pTexture->GetHandle(), depthDesc };
        }

        rhi::FramebufferHandle handle = m_FramebufferHandleManager.CreateHandle();
        m_pDevice->CreateFramebuffer( handle, desc );

        auto pFramebuffer = memory::CreateRef< Framebuffer >( handle, colorAttachments, depthAttachment );
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

        pFramebuffer->m_Width = width;
        pFramebuffer->m_Height = height;

        m_pDevice->InvalidateFramebuffer( pFramebuffer->m_Handle );
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

    GraphicsPipeline::Ref ResourceManager::CreateGraphicsPipeline( rhi::PrimitiveTopology topology,
        const rhi::BufferLayout &inputLayout,
        const rhi::RenderState &renderState,
        VertexShader::ConstRef pVertexShader,
        PixelShader::ConstRef pPixelShader,
        const primitive::Vector< rhi::BindingLayout > &bindingLayouts )
    {
        rhi::GraphicsPipelineDescriptor desc;
        desc.Topology = topology;
        desc.InputLayout = inputLayout;
        desc.State = renderState;
        desc.VertexShaderHandle = pVertexShader->m_Handle;
        desc.PixelShaderHandle = pPixelShader->m_Handle;
        desc.BindingLayouts = bindingLayouts;

        rhi::GraphicsPipelineHandle handle = m_GraphicsPipelineHandleManager.CreateHandle();
        m_pDevice->CreateGraphicsPipeline( handle, desc );

        auto pGraphicsPipeline = memory::CreateRef< GraphicsPipeline >(
            handle, topology, inputLayout, renderState, pVertexShader, pPixelShader, bindingLayouts );
        m_pGraphicsPipelines.PushBack( pGraphicsPipeline );
        return pGraphicsPipeline;
    }
}