/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/command_list.h"
#include "smile/graphic/rhi/resource/graphics_pipeline.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"
#include "smile/graphic/rhi/shader/binding_set.h"
#include "d3d11_context.h"

namespace smile::graphic::rhi
{
    class D3D11Device;
    class D3D11Pipeline;

    class D3D11CommandList final : public CommandList
    {
      public:
        D3D11CommandList( D3D11Device *pDevice, std::reference_wrapper< const D3D11Context > context );
        ~D3D11CommandList() = default;

        D3D11CommandList( const D3D11CommandList & ) = delete;
        D3D11CommandList( D3D11CommandList && ) = delete;
        D3D11CommandList &operator=( const D3D11CommandList & ) = delete;
        D3D11CommandList &operator=( D3D11CommandList && ) = delete;

        Object GetInternal( ObjectType type ) const override;

        void Open() override;
        void Close() override;
        void ClearState() override;

        void SetGraphicsState( const GraphicsState &graphicsState ) override;
        void Draw( const DrawParams &params ) override;
        void DrawIndexed( const DrawIndexedParams &params ) override;
        void DrawInstanced( const DrawInstancedParams &params ) override;
        void DrawIndexedInstanced( const DrawIndexedInstancedParams &params ) override;

        void FillBuffer( GPUBufferHandle handle, const void *pData, Uint32 size, Uint32 offset = 0 ) const override;

        void ClearTexture( TextureHandle handle,
            TextureSubresourceSet subresources,
            const math::Color &clearColor ) const override;

        void ClearDepthStencilTexture( TextureHandle handle,
            TextureSubresourceSet subresources,
            std::optional< float > depth,
            std::optional< Uint8 > stencil ) const override;

        void CopyTexture( TextureHandle dst,
            const TextureDescriptor &dstDesc,
            const TextureSlice &dstSlice,
            TextureHandle src,
            const TextureDescriptor &srcDesc,
            const TextureSlice &srcSlice ) const override;

        void CopyTexture( TextureHandle dst,
            const TextureSlice &dstSlice,
            TextureHandle src,
            const TextureSlice &srcSlice ) const override;

      private:
        void PrepareToBindGraphicsResourceSets( const BindingSetVector &resourceSets,
            const BindingSetVector *pCurrentResourceSets,
            GraphicsPipelineHandle currentPipelineHandle,
            GraphicsPipelineHandle newPipelineHandle,
            bool updateFramebuffer,
            BindingSetVector &outSetsToBind ) const;

        void BindGraphicsPipeline( const D3D11Pipeline &pipeline ) const;
        void BindGraphicsResourceSets( const BindingSetVector &setsToBind, const D3D11Pipeline &pipeline ) const;

      private:
        D3D11Device *m_pDevice;
        const D3D11Context &m_Context;

        GraphicsPipelineHandle m_CurrentGraphicsPipeline;
        FramebufferHandle m_CurrentFramebuffer;
        ViewportState m_CurrentViewport;
        BindingSetVector m_CurrentBindings;
        primitive::FixedVector< VertexBufferBinding, s_MaxVertexAttributeCount > m_CurrentVertexBufferBindings;
        IndexBufferBinding m_CurrentIndexBufferBinding;
        primitive::FixedVector< GPUBufferHandle, s_MaxVertexAttributeCount > m_CurrentVertexBuffers;
        GPUBufferHandle m_CurrentIndexBuffer;
        bool m_IsCurrentGraphicsStateValid = false;
    };
}
