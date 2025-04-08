/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/graphics_context.h"

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

        void SetState( const RenderState &state ) const override;
        void Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader ) override;
        void DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader ) override;

        void BindVertexBuffer( GPUBufferHandle handle, Uint32 stride ) const override;
        void UnbindVertexBuffer() const override;

        void BindIndexBuffer( GPUBufferHandle handle ) const override;
        void UnbindIndexBuffer() const override;

        void BindVertexShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const;
        void UnbindVertexShaderUniformBuffer( Uint16 slot ) const;
        void BindPixelShaderUniformBuffer( GPUBufferHandle handle, Uint16 slot ) const;
        void UnbindPixelShaderUniformBuffer( Uint16 slot ) const;

        void BindShader( const memory::Ref< Shader > &pShader ) const override;
        void UnbindShader() const override;

        void BindFramebuffer( FramebufferHandle handle ) const override;
        void ClearFramebuffer( FramebufferHandle handle ) override;

        void FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const override;
        void *ReadTexture( TextureHandle handle ) const override;
        void *ReadTexture( FramebufferHandle handle, Uint32 index ) const override;

      private:
        DirectX11Device *m_pDevice = nullptr;
        ID3D11DeviceContext *m_pInternal = nullptr;
    };
}
