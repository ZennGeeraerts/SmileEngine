/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "texture.h"
#include "frame_buffer.h"

#include "smile/graphic/renderer_backend/resource/frame_buffer.h"

#include <filesystem>

namespace smile::graphic
{
    class GraphicsDevice;

    class ResourceManager final
    {
      public:
        ResourceManager() = default;
        ~ResourceManager();

        void Initialize( GraphicsDevice *pDevice );

        VertexBuffer::Ref CreateVertexBuffer( void *pVertices, const Count vertexCount, const BufferLayout &layout );
        VertexBuffer::Ref CreateDynamicVertexBuffer( const Count vertexCount, const BufferLayout &layout );

        IndexBuffer::Ref CreateIndexBuffer( Uint32 *pIndices, const Count indexCount );

        ConstantBuffer::Ref CreateConstantBuffer( const BufferLayout &layout );

        VertexShader::Ref CreateVertexShader( const std::vector< Byte > &byteCode,
            const std::string &entryPoint,
            const std::string &targetProfile );

        PixelShader::Ref CreatePixelShader( const std::vector< Byte > &byteCode,
            const std::string &entryPoint,
            const std::string &targetProfile );

        memory::Ref< Texture > CreateTexture( const std::filesystem::path &path );
        memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor );

        void ResizeFramebuffer( memory::Ref< Framebuffer > pFramebuffer, Uint32 width, Uint32 height );

      private:
        GraphicsDevice *m_pDevice = nullptr;
        primitive::Vector< VertexBuffer::Ref > m_pVertexBuffers;
        primitive::Vector< IndexBuffer::Ref > m_pIndexBuffers;
        primitive::Vector< ConstantBuffer::Ref > m_pConstantBuffers;
        primitive::Vector< VertexShader::Ref > m_pVertexShaders;
        primitive::Vector< PixelShader::Ref > m_pPixelShaders;
        std::vector< memory::Ref< Texture > > m_pTextures;
        std::vector< memory::Ref< Framebuffer > > m_pFramebuffers;

        GPUBufferHandleManager m_GPUBufferHandleManager;
        TextureHandleManager m_TextureHandleManager;
        FramebufferHandleManager m_FramebufferHandleManager;
        ShaderHandleManager m_ShaderHandleManager;
    };
}