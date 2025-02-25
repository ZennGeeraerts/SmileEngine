/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource/vertex_buffer.h"
#include "resource/index_buffer.h"
#include "resource/texture.h"
#include "resource/frame_buffer.h"
#include "resource/rasterizer_state.h"
#include "shader/shader.h"

#include "memory/ref.h"

namespace smile::graphic
{
    class GraphicsDevice
    {
      public:
        GraphicsDevice() = default;
        virtual ~GraphicsDevice() = default;

        virtual void *GetInternal() const = 0;

        virtual memory::Ref< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc ) = 0;
        virtual memory::Ref< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc ) = 0;
        virtual memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) = 0;
        virtual memory::Ref< Shader > CreateShader( const std::string &assetFile,
            const std::string &techniqueName = "" ) = 0;
        virtual memory::Ref< Texture > CreateTexture2D( const std::string &filePath ) = 0;
        virtual memory::Ref< Texture > CreateTextureCube( const std::string &filePath ) = 0;
        virtual memory::Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) = 0;
        virtual memory::Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) = 0;

        virtual void InvalidateFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) = 0;
    };
}