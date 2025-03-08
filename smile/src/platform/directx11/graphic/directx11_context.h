/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
<<<<<<<< HEAD:smile/src/platform/directx11/graphic/directx11_context.h
#include "smile/graphic/renderer_api/graphics_context.h"
#include "directx11_swap_chain.h"
========
#include "engine/graphic/renderer_backend/graphics_context.h"
>>>>>>>> main:src/smile/platform/directx11/graphic/renderer_backend/directx11_context.h

#include <d3d11.h>

namespace smile::graphic
{
    class DirectX11Device;

    class DirectX11Context final : public GraphicsContext
    {
      public:
        DirectX11Context( DirectX11Device *pDevice, ID3D11DeviceContext *pInternal );
        ~DirectX11Context() = default;

        DirectX11Context( const DirectX11Context & ) = delete;
        DirectX11Context( DirectX11Context && ) = delete;
        DirectX11Context &operator=( const DirectX11Context & ) = delete;
        DirectX11Context &operator=( DirectX11Context && ) = delete;

        void *GetInternal() const override
        {
            return m_pInternal;
        }

        void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const override;
        void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain, const DirectX::XMFLOAT4 &clearColor ) const override;

        void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader ) override;
        void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader ) override;

        void BindVertexBuffer( VertexBufferHandle vbHandle ) const override;
        void UnbindVertexBuffer() const override;

        void BindIndexBuffer( IndexBufferHandle ibHandle ) const override;
        void UnbindIndexBuffer() const override;

        void BindShader( const memory::Ref< Shader > &pShader ) const override;
        void UnbindShader() const override;

        void BindFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer ) const override;
        void ClearFramebuffer( memory::Ref< Framebuffer > pFramebuffer ) override;

        void BindRasterizerState( const memory::Ref< RasterizerState > &pRasterizerState ) const override;
        void UnbindRasterizerState() const override;
        void BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const override;
        void UnbindPrimitiveTopology() const override;

        void FillVertexBuffer( VertexBufferHandle vbHandle, void *pData, Uint32 vertexCount ) const override;

      private:
        DirectX11Device *m_pDevice = nullptr;
        ID3D11DeviceContext *m_pInternal = nullptr;
    };
}
