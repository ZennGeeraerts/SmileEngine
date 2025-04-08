/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "resource_manager.h"

#include "smile/graphic/renderer_backend/graphics_device.h"

namespace smile::graphic
{
    ResourceManager::~ResourceManager()
    {
        for ( auto pVertexBuffer : m_pVertexBuffers )
            m_pDevice->DestroyGPUBuffer( pVertexBuffer->Handle );

        for ( auto pIndexBuffer : m_pIndexBuffers )
            m_pDevice->DestroyGPUBuffer( pIndexBuffer->Handle );

        for ( auto pUniformBuffer : m_pUniformBuffers )
            m_pDevice->DestroyGPUBuffer( pUniformBuffer->Handle );

        for ( auto pTexture : m_pTextures )
            m_pDevice->DestroyTexture( pTexture->Handle );
    }

    void ResourceManager::Initialize( GraphicsDevice *pDevice )
    {
        m_pDevice = pDevice;
    }

    memory::Ref< VertexBuffer >
    ResourceManager::CreateVertexBuffer( void *pVertices, Uint32 vertexCount, const VertexLayout &layout )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.pData = pVertices;
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = BufferUsage::Immutable;
        bufferDesc.CPUAccess = BufferCPUAccess::None;
        bufferDesc.BindFlags = BufferBindFlags::VertexBuffer;

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout );
        m_pVertexBuffers.push_back( pVertexBuffer );
        return pVertexBuffer;
    }

    memory::Ref< VertexBuffer > ResourceManager::CreateDynamicVertexBuffer( Uint32 vertexCount,
        const VertexLayout &layout )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.pData = nullptr;
        bufferDesc.Size = vertexCount * layout.GetStride();
        bufferDesc.Usage = BufferUsage::Dynamic;
        bufferDesc.CPUAccess = BufferCPUAccess::Write;
        bufferDesc.BindFlags = BufferBindFlags::VertexBuffer;

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, layout );
        m_pVertexBuffers.push_back( pVertexBuffer );
        return pVertexBuffer;
    }

    memory::Ref< IndexBuffer > ResourceManager::CreateIndexBuffer( Uint32 *pIndices, Uint32 indexCount )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.pData = pIndices;
        bufferDesc.Size = indexCount * sizeof( Uint32 );
        bufferDesc.Usage = BufferUsage::Immutable;
        bufferDesc.CPUAccess = BufferCPUAccess::None;
        bufferDesc.BindFlags = BufferBindFlags::IndexBuffer;

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pIndexBuffer = memory::CreateRef< IndexBuffer >( handle, indexCount );
        m_pIndexBuffers.push_back( pIndexBuffer );
        return pIndexBuffer;
    }

    memory::Ref< UniformBuffer >
    ResourceManager::CreateUniformBuffer( const std::string &name, void *pData, Uint32 size )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.pData = pData;
        bufferDesc.Size = size;
        bufferDesc.Usage = BufferUsage::Dynamic;
        bufferDesc.CPUAccess = BufferCPUAccess::Write;
        bufferDesc.BindFlags = BufferBindFlags::UniformBuffer;

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pUniformBuffer = memory::CreateRef< UniformBuffer >( handle, name, bufferDesc.Size );
        m_pUniformBuffers.push_back( pUniformBuffer );
        return pUniformBuffer;
    }

    memory::Ref< Shader > ResourceManager::CreateShader( const std::string &assetFile,
        const VertexLayout &layout,
        const std::string &techniqueName )
    {
        auto pShader = m_pDevice->CreateShader( assetFile, layout, techniqueName );
        m_pShaders.push_back( pShader );
        return pShader;
    }

    memory::Ref< Shader > ResourceManager::CreateShader( const std::string &assetFile,
        const std::string &techniqueName )
    {
        auto pShader = m_pDevice->CreateShader( assetFile, techniqueName );
        m_pShaders.push_back( pShader );
        return pShader;
    }

    memory::Ref< Texture > ResourceManager::CreateTexture( const std::filesystem::path &path )
    {
        TextureHandle handle = m_TextureHandleManager.CreateHandle();
        m_pDevice->CreateTexture( handle, path );

        auto pTexture = memory::CreateRef< Texture >( handle );
        m_pTextures.push_back( pTexture );
        return pTexture;
    }

    memory::Ref< Framebuffer > ResourceManager::CreateFramebuffer( const FramebufferDescriptor &descriptor )
    {
        auto pFramebuffer = m_pDevice->CreateFramebuffer( descriptor );
        m_pFramebuffers.push_back( pFramebuffer );
        return pFramebuffer;
    }

    void ResourceManager::ResizeFramebuffer( memory::Ref< Framebuffer > pFramebuffer, Uint32 width, Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > pFramebuffer->MaxFramebufferSize ) ||
             ( height > pFramebuffer->MaxFramebufferSize ) )
        {
            SM_LOG_WARNING( "ResourceManager::ResizeFramebuffer > Invalid framebuffer size: {0}, {1}", width, height );
            return;
        }

        pFramebuffer->Descriptor.Width = width;
        pFramebuffer->Descriptor.Height = height;

        m_pDevice->InvalidateFramebuffer( pFramebuffer );
    }
}