/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "resource_manager.h"

namespace smile::graphic
{
    void ResourceManager::Initialize( GraphicsDevice *pDevice )
    {
        m_pDevice = pDevice;
    }

    memory::Ref< VertexBuffer > ResourceManager::CreateVertexBuffer( const GPUBufferDescriptor &bufferDesc,
        Uint32 stride )
    {
        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pVertexBuffer = memory::CreateRef< VertexBuffer >( handle, stride );
        m_pVertexBuffers.push_back( pVertexBuffer );
        return pVertexBuffer;
    }

    memory::Ref< IndexBuffer > ResourceManager::CreateIndexBuffer( const GPUBufferDescriptor &bufferDesc, Uint32 count )
    {
        GPUBufferHandle handle = m_GPUBufferHandleManager.CreateHandle();
        m_pDevice->CreateGPUBuffer( handle, bufferDesc );

        auto pIndexBuffer = memory::CreateRef< IndexBuffer >( handle, count );
        m_pIndexBuffers.push_back( pIndexBuffer );
        return pIndexBuffer;
    }

    memory::Ref< UniformBuffer > ResourceManager::CreateUniformBuffer( const GPUBufferDescriptor &bufferDesc,
        const std::string &name )
    {
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

    memory::Ref< Texture > ResourceManager::CreateTexture2D( const std::string &filePath )
    {
        auto pTexture = m_pDevice->CreateTexture2D( filePath );
        m_pTextures2D.push_back( pTexture );
        return pTexture;
    }

    memory::Ref< Texture > ResourceManager::CreateTextureCube( const std::string &filePath )
    {
        auto pTexture = m_pDevice->CreateTextureCube( filePath );
        m_pTexturesCube.push_back( pTexture );
        return pTexture;
    }

    memory::Ref< Framebuffer > ResourceManager::CreateFramebuffer( const FramebufferDescriptor &descriptor )
    {
        auto pFramebuffer = m_pDevice->CreateFramebuffer( descriptor );
        m_pFramebuffers.push_back( pFramebuffer );
        return pFramebuffer;
    }

    memory::Ref< RasterizerState > ResourceManager::CreateRasterizerState( const RasterizerStateDescriptor &descriptor )
    {
        auto pRasterizerState = m_pDevice->CreateRasterizerState( descriptor );
        m_pRasterizerStates.push_back( pRasterizerState );
        return pRasterizerState;
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