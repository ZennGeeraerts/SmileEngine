/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "memory/ref.h"

#include "engine/graphic/renderer_api/graphics_device.h"

#include <vector>

namespace smile::graphic
{
    class ResourceManager final
    {
      public:
        ResourceManager() = default;
        ~ResourceManager() = default;

        void Initialize( GraphicsDevice *pDevice );

        memory::Ref< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc );
        memory::Ref< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc );
        memory::Ref< Shader >
        CreateShader( const std::string &assetFile, const BufferLayout &layout, const std::string &techniqueName = "" );
        memory::Ref< Shader > CreateShader( const std::string &assetFile, const std::string &techniqueName = "" );
        memory::Ref< Texture > CreateTexture2D( const std::string &filePath );
        memory::Ref< Texture > CreateTextureCube( const std::string &filePath );
        memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor );
        memory::Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor );

        void ResizeFramebuffer( memory::Ref< Framebuffer > pFramebuffer, Uint32 width, Uint32 height );

      private:
        GraphicsDevice *m_pDevice;
        std::vector< memory::Ref< VertexBuffer > > m_pVertexBuffers;
        std::vector< memory::Ref< IndexBuffer > > m_pIndexBuffers;
        std::vector< memory::Ref< Shader > > m_pShaders;
        std::vector< memory::Ref< Texture > > m_pTextures2D;
        std::vector< memory::Ref< Texture > > m_pTexturesCube;
        std::vector< memory::Ref< Framebuffer > > m_pFramebuffers;
        std::vector< memory::Ref< RasterizerState > > m_pRasterizerStates;
    };
}