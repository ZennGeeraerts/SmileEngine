/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "memory/ref.h"

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "uniform_buffer.h"

#include "smile/graphic/renderer_backend/graphics_device.h"

#include <vector>

namespace smile::graphic
{
    class ResourceManager final
    {
      public:
        ResourceManager() = default;
        ~ResourceManager() = default;

        void Initialize( GraphicsDevice *pDevice );

        memory::Ref< VertexBuffer > CreateVertexBuffer( const GPUBufferDescriptor &bufferDesc, Uint32 stride );
        memory::Ref< IndexBuffer > CreateIndexBuffer( const GPUBufferDescriptor &bufferDesc, Uint32 count );
        memory::Ref< UniformBuffer > CreateUniformBuffer( const GPUBufferDescriptor &bufferDesc,
            const std::string &name );

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
        std::vector< memory::Ref< UniformBuffer > > m_pUniformBuffers;
        std::vector< memory::Ref< Shader > > m_pShaders;
        std::vector< memory::Ref< Texture > > m_pTextures2D;
        std::vector< memory::Ref< Texture > > m_pTexturesCube;
        std::vector< memory::Ref< Framebuffer > > m_pFramebuffers;
        std::vector< memory::Ref< RasterizerState > > m_pRasterizerStates;

        GPUBufferHandleManager m_GPUBufferHandleManager;
    };
}