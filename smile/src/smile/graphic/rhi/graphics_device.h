/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "command_list.h"
#include "render_handle.h"
#include "object.h"

#include "resource/buffer.h"
#include "resource/texture.h"
#include "resource/frame_buffer.h"
#include "resource/sampler.h"
#include "resource/graphics_pipeline.h"
#include "shader/shader.h"
#include "shader/binding_set.h"

namespace smile::graphic::rhi
{
    enum class RendererBackendType
    {
        None = 0,
        D3D11 = 1
    };

    template < typename CollectionType >
    concept HasIsValidIndex = requires( const CollectionType &collection, const Index index ) {
        { collection.IsValidIndex( index ) } -> std::convertible_to< bool >;
    };

    class GraphicsDevice
    {
      public:
        GraphicsDevice() = default;
        virtual ~GraphicsDevice() = default;

        virtual Object GetInternal( ObjectType type ) const = 0;

        virtual CommandList *CreateCommandList() = 0;

        virtual Scope< SwapChain > CreateSwapChain( const window::Window *pWindow ) = 0;

        virtual void
        CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc, void *pData = nullptr ) = 0;
        virtual void DestroyGPUBuffer( GPUBufferHandle handle ) = 0;

        virtual void CreateBindingSet( BindingSetHandle handle,
            const BindingSetDescriptor &bindingSetDesc,
            const BindingLayout &layout ) = 0;
        virtual void DestroyBindingSet( BindingSetHandle handle ) = 0;

        virtual void CreateShader( ShaderHandle handle,
            const ShaderDescriptor &shaderDesc,
            const primitive::Vector< Byte > &byteCode ) = 0;
        virtual void DestroyShader( ShaderHandle handle ) = 0;

        virtual void CreateGraphicsPipeline( GraphicsPipelineHandle handle,
            const GraphicsPipelineDescriptor &pipelineDesc ) = 0;
        virtual void DestroyGraphicsPipeline( GraphicsPipelineHandle handle ) = 0;

        virtual void CreateTexture( TextureHandle handle,
            const TextureDescriptor &desc,
            const primitive::Vector< Byte > &buffer = {} ) = 0;
        virtual void DestroyTexture( TextureHandle handle ) = 0;

        virtual void CreateStagingTexture( StagingTextureHandle handle, const TextureDescriptor &desc ) = 0;
        virtual void DestroyStagingTexture( StagingTextureHandle handle ) = 0;
        virtual void *
        MapStagingTexture( StagingTextureHandle handle, const TextureSlice &slice, CPUAccessMode cpuAccess ) = 0;
        virtual void UnmapStagingTexture( StagingTextureHandle handle ) = 0;

        virtual void CreateHandleForNativeTexture( TextureHandle handle,
            Object nativeTexture,
            ObjectType type,
            const TextureDescriptor &desc ) = 0;

        virtual Object GetNativeView( TextureHandle handle,
            ObjectType type,
            Format format,
            const TextureSubresourceSet &subresources,
            TextureDimension dimension,
            bool isReadOnlyDSV = false ) = 0;

        virtual void CreateSampler( SamplerHandle handle, const SamplerDescriptor &samplerDesc ) = 0;
        virtual void DestroySampler( SamplerHandle handle ) = 0;

        virtual void CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor ) = 0;
        virtual void DestroyFramebuffer( FramebufferHandle handle ) = 0;

        static Scope< GraphicsDevice > Create( RendererBackendType backendType );

      protected:
        template < HasIsValidIndex CollectionType >
        bool IsHandleValid( primitive::Handle< Uint64, 32, 32 > handle, const CollectionType &collection )
        {
            return handle.IsValid() && collection.IsValidIndex( handle.GetIndex() );
        }
    };
}