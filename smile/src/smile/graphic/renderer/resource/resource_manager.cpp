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

    memory::Ref< IndexBuffer > ResourceManager::CreateIndexBuffer( Uint32 *pIndices, Uint32 indexCount )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = indexCount * sizeof( Uint32 );
        bufferDesc.Usage = BufferUsage::Immutable;
        bufferDesc.CPUAccess = CPUAccessMode::None;
        bufferDesc.BindFlags = { BufferBindFlags::IndexBuffer };

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pIndices );

        auto pIndexBuffer = memory::CreateRef< IndexBuffer >( handle, indexCount );
        m_pIndexBuffers.push_back( pIndexBuffer );
        return pIndexBuffer;
    }

    memory::Ref< UniformBuffer >
    ResourceManager::CreateUniformBuffer( const std::string &name, void *pData, Uint32 size )
    {
        GPUBufferDescriptor bufferDesc{};
        bufferDesc.Size = size;
        bufferDesc.Usage = BufferUsage::Dynamic;
        bufferDesc.CPUAccess = CPUAccessMode::Write;
        bufferDesc.BindFlags = { BufferBindFlags::UniformBuffer };

        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc, pData );

        auto pUniformBuffer = memory::CreateRef< UniformBuffer >( handle, name, bufferDesc.Size );
        m_pUniformBuffers.push_back( pUniformBuffer );
        return pUniformBuffer;
    }

    memory::Ref< Shader > ResourceManager::CreateShader( const std::string &assetFile,
        const BufferLayout &layout,
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
        FramebufferHandle handle = m_FramebufferHandleManager.CreateHandle();
        m_pDevice->CreateFramebuffer( handle, descriptor );

        auto pFramebuffer = memory::CreateRef< Framebuffer >( handle );
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

        pFramebuffer->Width = width;
        pFramebuffer->Height = height;

        m_pDevice->InvalidateFramebuffer( pFramebuffer->Handle );
    }
}