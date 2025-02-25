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

    memory::Ref< VertexBuffer > ResourceManager::CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc )
    {
        auto pVertexBuffer = m_pDevice->CreateVertexBuffer( vertexBufferDesc );
        m_pVertexBuffers.push_back( pVertexBuffer );
        return pVertexBuffer;
    }

    memory::Ref< IndexBuffer > ResourceManager::CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc )
    {
        auto pIndexBuffer = m_pDevice->CreateIndexBuffer( indexBufferDesc );
        m_pIndexBuffers.push_back( pIndexBuffer );
        return pIndexBuffer;
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
}