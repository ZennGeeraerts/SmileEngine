/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/command_list.h"
#include "smile/graphic/rhi/rhi.h"
#include "directx11_context.h"

namespace smile::graphic
{
    class DirectX11Device;
    class DirectX11Pipeline;

    class DirectX11CommandList final : public CommandList
    {
      public:
        DirectX11CommandList( DirectX11Device *pDevice, std::reference_wrapper< const DirectX11Context > context );
        ~DirectX11CommandList() = default;

        DirectX11CommandList( const DirectX11CommandList & ) = delete;
        DirectX11CommandList( DirectX11CommandList && ) = delete;
        DirectX11CommandList &operator=( const DirectX11CommandList & ) = delete;
        DirectX11CommandList &operator=( DirectX11CommandList && ) = delete;

        void *GetInternal() const override
        {
            return m_Context.pImmediateContext;
        }

        void Open() override;
        void Close() override;
        void ClearState() override;

        void BindBackBuffer( memory::Ref< SwapChain > pSwapChain ) const override;
        void ClearBackBuffer( memory::Ref< SwapChain > pSwapChain, const DirectX::XMFLOAT4 &clearColor ) const override;

        void SetGraphicsState( const GraphicsState &graphicsState ) override;
        void Draw( const DrawParams &params ) override;
        void DrawIndexed( const DrawIndexedParams &params ) override;

        void FillBuffer( GPUBufferHandle handle, void *pData, Uint32 size ) const override;
        void *ReadTexture( TextureHandle handle ) override;
        void *ReadTexture( FramebufferHandle handle, Uint32 index ) const override;

      private:
        void PrepareToBindGraphicsResourceSets( const BindingSetVector &resourceSets,
            const BindingSetVector *pCurrentResourceSets,
            GraphicsPipelineHandle currentPipelineHandle,
            GraphicsPipelineHandle newPipelineHandle,
            bool updateFramebuffer,
            BindingSetVector &outSetsToBind ) const;

        void BindGraphicsPipeline( const DirectX11Pipeline &pipeline ) const;
        void BindGraphicsResourceSets( const BindingSetVector &setsToBind, const DirectX11Pipeline &pipeline ) const;

      private:
        DirectX11Device *m_pDevice;
        const DirectX11Context &m_Context;

        GraphicsPipelineHandle m_CurrentGraphicsPipeline;
        FramebufferHandle m_CurrentFramebuffer;
        BindingSetVector m_CurrentBindings;
        primitive::FixedVector< VertexBufferBinding, s_MaxVertexAttributeCount > m_CurrentVertexBufferBindings;
        IndexBufferBinding m_CurrentIndexBufferBinding;
        primitive::FixedVector< GPUBufferHandle, s_MaxVertexAttributeCount > m_CurrentVertexBuffers;
        GPUBufferHandle m_CurrentIndexBuffer;
        bool m_IsCurrentGraphicsStateValid = false;
    };
}
