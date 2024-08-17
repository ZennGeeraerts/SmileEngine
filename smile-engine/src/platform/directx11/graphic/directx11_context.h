/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/graphic/renderer_api/graphics_context.h"
#include "directx11_swap_chain.h"

#include <d3d11.h>

namespace smile::graphic
{
    class DirectX11Context final : public GraphicsContext
    {
      public:
        DirectX11Context() = default;
        virtual ~DirectX11Context();

        DirectX11Context( const DirectX11Context & ) = delete;
        DirectX11Context( DirectX11Context && ) = delete;
        DirectX11Context &operator=( const DirectX11Context & ) = delete;
        DirectX11Context &operator=( DirectX11Context && ) = delete;

        void *GetInternal() const override
        {
            return m_pInternal;
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
        ID3D11DeviceContext *m_pInternal = nullptr;
        DirectX11SwapChain *m_pSwapChain = nullptr;

        friend class DirectX11Device;
        friend class DirectX11RendererAPI;
    };
}
