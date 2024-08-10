/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/graphic/graphics_context.h"
#include "smile_raster_swap_chain.h"

#include <device_context.cuh>

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class SmileRasterContext final : public GraphicsContext
    {
      public:
        SmileRasterContext();
        virtual ~SmileRasterContext();

        SmileRasterContext( const SmileRasterContext & ) = delete;
        SmileRasterContext( SmileRasterContext && ) = delete;
        SmileRasterContext &operator=( const SmileRasterContext & ) = delete;
        SmileRasterContext &operator=( SmileRasterContext && ) = delete;

        void *GetInternal() const override
        {
            return m_pDeviceContext;
        }

        void Draw( Uint32 vertexCount, const Ref< Shader > &pShader ) override;
        void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) override;
        void Clear( const DirectX::XMFLOAT4 &clearColor ) override;

        void BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const override;
        void UnbindVertexBuffer() const override;
        void BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const override;
        void UnbindIndexBuffer() const override;
        void BindShader( const Ref< Shader > &pShader ) const override;
        void UnbindShader() const override;
        void BindFramebuffer( const Ref< Framebuffer > &pFramebuffer ) const override;
        void UnbindFramebuffer() const override;
        void ClearFramebuffer( const Ref< Framebuffer > &pFramebuffer ) override;
        void BindRasterizerState( const Ref< RasterizerState > &pRasterizerState ) const override;
        void UnbindRasterizerState() const override;
        void BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const override;
        void UnbindPrimitiveTopology() const override;

        void
        FillVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer, void *pData, Uint32 vertexCount ) const override;

      private:
        Raster::DeviceContext *m_pDeviceContext = nullptr;
        SmileRasterSwapChain *m_pSwapChain = nullptr;

        friend class SmileRasterRendererAPI;
    };
}
