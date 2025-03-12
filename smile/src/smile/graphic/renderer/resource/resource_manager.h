/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "uniform_buffer.h"
#include "rasterizer_state.h"
#include "smile/graphic/renderer_backend/resource/buffer.h"
#include "smile/graphic/renderer_backend/resource/frame_buffer.h"
#include "smile/graphic/renderer_backend/resource/rasterizer_state.h"
#include "smile/graphic/renderer_backend/shader/shader.h"

#include <vector>

namespace smile::graphic
{
    class GraphicsDevice;

    class ResourceManager final
    {
      public:
        ResourceManager() = default;
        ~ResourceManager() = default;

        void Initialize( GraphicsDevice *pDevice );

        memory::Ref< VertexBuffer >
        CreateVertexBuffer( void *pVertices, Uint32 vertexCount, const VertexLayout &layout );
        memory::Ref< VertexBuffer > CreateDynamicVertexBuffer( Uint32 vertexCount, const VertexLayout &layout );

        memory::Ref< IndexBuffer > CreateIndexBuffer( Uint32 *pIndices, Uint32 indexCount );

        memory::Ref< UniformBuffer > CreateUniformBuffer( const std::string &name, void *pData, Uint32 size );

        memory::Ref< Shader >
        CreateShader( const std::string &assetFile, const VertexLayout &layout, const std::string &techniqueName = "" );
        memory::Ref< Shader > CreateShader( const std::string &assetFile, const std::string &techniqueName = "" );
        memory::Ref< Texture > CreateTexture2D( const std::string &filePath );
        memory::Ref< Texture > CreateTextureCube( const std::string &filePath );
        memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor );
        memory::Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor );

        void ResizeFramebuffer( memory::Ref< Framebuffer > pFramebuffer, Uint32 width, Uint32 height );

      private:
        GraphicsDevice *m_pDevice = nullptr;
        std::vector< memory::Ref< VertexBuffer > > m_pVertexBuffers;
        std::vector< memory::Ref< IndexBuffer > > m_pIndexBuffers;
        std::vector< memory::Ref< UniformBuffer > > m_pUniformBuffers;
        std::vector< memory::Ref< Shader > > m_pShaders;
        std::vector< memory::Ref< Texture > > m_pTextures2D;
        std::vector< memory::Ref< Texture > > m_pTexturesCube;
        std::vector< memory::Ref< Framebuffer > > m_pFramebuffers;
        std::vector< memory::Ref< RasterizerState > > m_pRasterizerStates;

        GPUBufferHandleManager m_GPUBufferHandleManager;
        RasterizerStateHandleManager m_RasterizerStateHandleManager;
    };
}