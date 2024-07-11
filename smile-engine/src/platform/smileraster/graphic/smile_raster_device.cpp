/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile_raster_device.h"

#include "resource/smile_raster_vertex_buffer.h"
#include "resource/smile_raster_index_buffer.h"
#include "platform/sdl/graphic/resource/sdl_texture.h"

#include "shader/smileraster_shader.h"

namespace smile::graphic
{
    SmileRasterDevice::SmileRasterDevice( Raster::DeviceContext *pContext ) : m_pDeviceContext{ pContext }
    {
    }

    Ref< VertexBuffer > SmileRasterDevice::CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc )
    {
        Ref< SmileRasterVertexBuffer > pVertexBuffer = CreateRef< SmileRasterVertexBuffer >();
        pVertexBuffer->Stride = vertexBufferDesc.Stride;
        pVertexBuffer->Internal = m_pDeviceContext->CreateVertexBuffer(
            vertexBufferDesc.pVertices, vertexBufferDesc.Count, vertexBufferDesc.Stride * vertexBufferDesc.Count );

        return pVertexBuffer;
    }

    Ref< IndexBuffer > SmileRasterDevice::CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc )
    {
        Ref< SmileRasterIndexBuffer > pIndexBuffer = CreateRef< SmileRasterIndexBuffer >();
        pIndexBuffer->Count = indexBufferDesc.Count;
        pIndexBuffer->Internal = m_pDeviceContext->CreateIndexBuffer( indexBufferDesc.pIndices, indexBufferDesc.Count );

        return pIndexBuffer;
    }

    Ref< Shader > SmileRasterDevice::CreateShader( const std::string &assetFile,
        const BufferLayout &layout,
        const std::string &techniqueName )
    {
        Ref< SmileRasterShader > pShader = CreateRef< SmileRasterShader >();
        pShader->BufferLayout = layout;
        pShader->pDeviceContext = m_pDeviceContext;
        pShader->SetName( assetFile );

        return pShader;
    }

    Ref< Shader > SmileRasterDevice::CreateShader( const std::string &assetFile, const std::string &techniqueName )
    {
        return Ref< Shader >();
    }

    Ref< Texture2D > SmileRasterDevice::CreateTexture2D( const std::string &filePath )
    {
        Ref< SDLTexture2D > pTexture = CreateRef< SDLTexture2D >();
        pTexture->pSurface = IMG_Load( filePath.c_str() );
        pTexture->FilePath = filePath;

        SM_ASSERT( pTexture->pSurface, "SmileRasterGraphicsDevice::CreateTexture2D > Failed to load image" );

        pTexture->pPixels = static_cast< Uint8 * >( pTexture->pSurface->pixels );
        pTexture->Width = pTexture->pSurface->w;
        pTexture->Height = pTexture->pSurface->h;

        pTexture->Internal = m_pDeviceContext->CreateTexture2D( pTexture->pPixels, pTexture->Width, pTexture->Height );

        return pTexture;
    }

    Ref< TextureCube > SmileRasterDevice::CreateTextureCube( const std::string &filePath )
    {
        return Ref< TextureCube >();
    }

    Ref< Framebuffer > SmileRasterDevice::CreateFramebuffer( const FramebufferDescriptor &descriptor )
    {
        return Ref< Framebuffer >();
    }

    Ref< RasterizerState > SmileRasterDevice::CreateRasterizerState( const RasterizerStateDescriptor &descriptor )
    {
        return Ref< RasterizerState >();
    }

    void SmileRasterDevice::InvalidateFramebuffer( const Ref< Framebuffer > &pFramebuffer )
    {
    }
    void SmileRasterDevice::ResizeFramebuffer( const Ref< Framebuffer > &pFramebuffer, Uint32 width, Uint32 height )
    {
    }
}