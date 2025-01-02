/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smileraster_context.h"

#include "smile/engine/core/window/window.h"
#include "smile/engine/common/logger/logger.h"

namespace smile::graphic
{
    SmileRasterContext::SmileRasterContext()
    {
        Raster::RenderConfig renderConfig{};
        m_pDeviceContext = new Raster::DeviceContext{ renderConfig };
    }

    SmileRasterContext::~SmileRasterContext()
    {
        delete m_pDeviceContext;
    }

    void SmileRasterContext::Draw( Uint32 vertexCount, const Ref< Shader > &pShader )
    {
    }

    void SmileRasterContext::DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader )
    {
        m_pDeviceContext->DrawIndexed( indexCount );
    }

    void SmileRasterContext::Clear( const DirectX::XMFLOAT4 &clearColor )
    {
        m_pDeviceContext->Clear( m_pSwapChain->GetFramebuffer(), clearColor, true );
    }

    void SmileRasterContext::BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const
    {
        Raster::BufferID bufferID = reinterpret_cast< Raster::BufferID >( pVertexBuffer->GetInternal() );
        m_pDeviceContext->BindVertexBuffer( bufferID, pVertexBuffer->Stride );
    }

    void SmileRasterContext::UnbindVertexBuffer() const
    {
        m_pDeviceContext->BindVertexBuffer( SMR_INVALID_BUFFER_ID, 0 );
    }

    void SmileRasterContext::BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const
    {
        Raster::BufferID bufferID = reinterpret_cast< Raster::BufferID >( pIndexBuffer->GetInternal() );
        m_pDeviceContext->BindIndexBuffer( bufferID );
    }

    void SmileRasterContext::UnbindIndexBuffer() const
    {
        m_pDeviceContext->BindIndexBuffer( SMR_INVALID_BUFFER_ID );
    }

    void SmileRasterContext::BindShader( const Ref< Shader > &pShader ) const
    {
    }

    void SmileRasterContext::UnbindShader() const
    {
    }

    void SmileRasterContext::BindFramebuffer( const Ref< Framebuffer > &pFramebuffer ) const
    {
    }

    void SmileRasterContext::UnbindFramebuffer() const
    {
    }

    void SmileRasterContext::ClearFramebuffer( const Ref< Framebuffer > &pFramebuffer )
    {
    }

    void SmileRasterContext::BindRasterizerState( const Ref< RasterizerState > &pRasterizerState ) const
    {
    }

    void SmileRasterContext::UnbindRasterizerState() const
    {
    }

    void SmileRasterContext::BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const
    {
    }

    void SmileRasterContext::UnbindPrimitiveTopology() const
    {
    }

    void SmileRasterContext::FillVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer,
        void *pData,
        Uint32 vertexCount ) const
    {
    }
}