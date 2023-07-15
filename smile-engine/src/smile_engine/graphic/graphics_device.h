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

namespace smile::graphic
{
    class GraphicsContext;

    class GraphicsDevice
    {
      public:
        GraphicsDevice() = default;
        virtual ~GraphicsDevice() = default;

        virtual void Initialize( GraphicsContext *pGraphicsContext ) = 0;

        virtual void *GetInternal() const = 0;

        virtual Ref< VertexBuffer > CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc ) = 0;
        virtual Ref< IndexBuffer > CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc ) = 0;
        virtual Ref< Shader > CreateShader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" ) = 0;
        virtual Ref< Shader > CreateShader( const std::string &assetFile, const std::string &techniqueName = "" ) = 0;
        virtual Ref< Texture2D > CreateTexture2D( const std::string &filePath ) = 0;
        virtual Ref< TextureCube > CreateTextureCube( const std::string &filePath ) = 0;
        virtual Ref< Framebuffer > CreateFramebuffer( const FramebufferDescriptor &descriptor ) = 0;
        virtual Ref< RasterizerState > CreateRasterizerState( const RasterizerStateDescriptor &descriptor ) = 0;

        virtual void InvalidateFramebuffer( const Ref< Framebuffer > &pFramebuffer ) = 0;
        virtual void ResizeFramebuffer( const Ref< Framebuffer > &pFramebuffer, Uint32 width, Uint32 height ) = 0;

        static GraphicsDevice *Create();
    };
}