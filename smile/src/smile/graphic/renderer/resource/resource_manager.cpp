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
            m_pDevice->DestroyFramebuffer( pFramebuffer->Handle );
    }

    void ResourceManager::Initialize( GraphicsDevice *pDevice )
    {
        m_pDevice = pDevice;
    }

    VertexBuffer::Ref
    ResourceManager::CreateVertexBuffer( void *pVertices, const Count vertexCount, const BufferLayout &layout )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = BufferUsage::Immutable;
        bufferDesc.CPUAccess = CPUAccessMode::None;
        bufferDesc.BindFlags = { BufferBindFlags::VertexBuffer };

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pVertices );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout, vertexCount );
        m_pVertexBuffers.PushBack( pVertexBuffer );
        return pVertexBuffer;
    }

    VertexBuffer::Ref ResourceManager::CreateDynamicVertexBuffer( const Count vertexCount, const BufferLayout &layout )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = BufferUsage::Dynamic;
        bufferDesc.CPUAccess = CPUAccessMode::Write;
        bufferDesc.BindFlags = { BufferBindFlags::VertexBuffer };

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout );
        m_pVertexBuffers.PushBack( pVertexBuffer );
        return pVertexBuffer;
    }

    memory::Ref< IndexBuffer > ResourceManager::CreateIndexBuffer( Uint32 *pIndices, const Count indexCount )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = indexCount * sizeof( Uint32 );
        bufferDesc.Usage = BufferUsage::Immutable;
        bufferDesc.CPUAccess = CPUAccessMode::None;
        bufferDesc.BindFlags = { BufferBindFlags::IndexBuffer };

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pIndices );

        auto pIndexBuffer = memory::CreateRef< IndexBuffer >( handle, indexCount );
        m_pIndexBuffers.PushBack( pIndexBuffer );
        return pIndexBuffer;
    }

    Texture::Ref ResourceManager::CreateTexture2D( Image::ConstRef pImage, bool updateable )
    {
        TextureDescriptor textureDesc{};
        textureDesc.Width = pImage->GetWidth();
        textureDesc.Height = pImage->GetHeight();
        textureDesc.TextureFormat = pImage->GetFormat();
        textureDesc.Dimension = TextureDimension::Texture2D;
        textureDesc.BindFlags = { TextureBindFlags::ShaderResource };
        textureDesc.CPUAccess = updateable ? CPUAccessMode::Write : CPUAccessMode::Read;

        TextureHandle handle = m_TextureHandleManager.CreateHandle();

        auto buffer = std::vector< Byte >{ pImage->GetData(), pImage->GetData() + pImage->GetDataSize() };
        m_pDevice->CreateTexture( handle, textureDesc, buffer );

        auto pTexture = memory::CreateRef< Texture >( handle, pImage->GetWidth(), pImage->GetHeight() );
        m_pTextures.PushBack( pTexture );
        return pTexture;
    }

    Texture::Ref ResourceManager::CreateTextureCube( Image::ConstRef pImage, bool updateable )
    {
        TextureDescriptor textureDesc{};
        textureDesc.Width = pImage->GetWidth();
        textureDesc.Height = pImage->GetHeight();
        textureDesc.TextureFormat = pImage->GetFormat();
        textureDesc.Dimension = TextureDimension::TextureCube;
        textureDesc.BindFlags = { TextureBindFlags::ShaderResource };
        textureDesc.CPUAccess = updateable ? CPUAccessMode::Write : CPUAccessMode::Read;

        TextureHandle handle = m_TextureHandleManager.CreateHandle();

        auto buffer = std::vector< Byte >{ pImage->GetData(), pImage->GetData() + pImage->GetDataSize() };
        m_pDevice->CreateTexture( handle, textureDesc, buffer );

        auto pTexture = memory::CreateRef< Texture >( handle, pImage->GetWidth(), pImage->GetHeight() );
        m_pTextures.PushBack( pTexture );
        return pTexture;
    }

    ConstantBuffer::Ref ResourceManager::CreateConstantBuffer( const BufferLayout &layout )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = layout.GetStride();
        bufferDesc.Usage = BufferUsage::Dynamic;
        bufferDesc.CPUAccess = CPUAccessMode::Write;
        bufferDesc.BindFlags = { BufferBindFlags::ConstantBuffer };

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pConstantBuffer = memory::CreateRef< ConstantBuffer >( handle, bufferDesc.Size );
        m_pConstantBuffers.PushBack( pConstantBuffer );
        return pConstantBuffer;
    }

    VertexShader::Ref ResourceManager::CreateVertexShader( const std::vector< Byte > &byteCode,
        const std::string &entryPoint,
        const std::string &targetProfile )
    {
        ShaderDescriptor shaderDesc{ ShaderStage::Vertex };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        auto pVertexShader = memory::CreateRef< VertexShader >( handle );
        m_pVertexShaders.PushBack( pVertexShader );
        return pVertexShader;
    }

    PixelShader::Ref ResourceManager::CreatePixelShader( const std::vector< Byte > &byteCode,
        const std::string &entryPoint,
        const std::string &targetProfile )
    {
        ShaderDescriptor shaderDesc{ ShaderStage::Pixel };
        shaderDesc.EntryPoint = entryPoint;
        shaderDesc.TargetProfile = targetProfile;

        ShaderHandle handle = m_ShaderHandleManager.CreateHandle();
        m_pDevice->CreateShader( handle, shaderDesc, byteCode );

        auto pPixelShader = memory::CreateRef< PixelShader >( handle );
        m_pPixelShaders.PushBack( pPixelShader );
        return pPixelShader;
    }

    Framebuffer::Ref ResourceManager::CreateFramebuffer( const FramebufferDescriptor &descriptor )
    {
        FramebufferHandle handle = m_FramebufferHandleManager.CreateHandle();
        m_pDevice->CreateFramebuffer( handle, descriptor );

        auto pFramebuffer = memory::CreateRef< Framebuffer >( handle, descriptor.Width, descriptor.Height );
        m_pFramebuffers.PushBack( pFramebuffer );
        return pFramebuffer;
    }

    void ResourceManager::ResizeFramebuffer( memory::Ref< Framebuffer > pFramebuffer, Uint32 width, Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > s_MaxFramebufferCount ) ||
             ( height > s_MaxFramebufferCount ) )
        {
            SM_LOG_WARNING( "ResourceManager::ResizeFramebuffer > Invalid framebuffer size: {0}, {1}", width, height );
            return;
        }

        pFramebuffer->m_Width = width;
        pFramebuffer->m_Height = height;

        m_pDevice->InvalidateFramebuffer( pFramebuffer->m_Handle );
    }
}