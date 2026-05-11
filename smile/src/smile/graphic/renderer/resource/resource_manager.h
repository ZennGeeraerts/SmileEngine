/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/array.h"

#include "smile/graphic/resource/image.h"

#include "smile/graphic/shader/shader_asset.h"

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "sampler.h"
#include "frame_buffer.h"
#include "graphics_pipeline.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "smile/graphic/renderer/shader/binding_layout.h"
#include "smile/graphic/renderer/shader/binding_set.h"
#include "smile/graphic/renderer/sprite/texture_asset.h"

#include "smile/graphic/rhi/object.h"

namespace smile::graphic
{
    namespace rhi
    {
        class GraphicsDevice;
    }

    class ResourceManager final
    {
      public:
        ResourceManager( rhi::GraphicsDevice &device ) noexcept;
        ~ResourceManager();

        VertexBuffer CreateVertexBuffer( void *pVertices, const Count vertexCount, const rhi::BufferLayout &layout );
        VertexBuffer CreateDynamicVertexBuffer( const Count vertexCount, const rhi::BufferLayout &layout );
        void DestroyVertexBuffer( VertexBuffer &vertexBuffer );

        IndexBuffer CreateIndexBuffer( Uint32 *pIndices, const Count indexCount );
        void DestroyIndexBuffer( IndexBuffer &indexBuffer );

        Texture CreateTexture2D( Image::ConstRef pImage, bool updateable );
        Texture CreateTexture2D( TextureAsset::ConstRef textureAsset );

        Texture GetOrCreateTexture2D( TextureAsset::ConstRef textureAsset );

        Texture CreateTextureCube( Image::ConstRef pImage, bool updateable );

        Texture
        CreateTextureFromNative( rhi::Object nativeTexture, rhi::ObjectType type, const rhi::TextureDescriptor &desc );

        void DestroyTexture( Texture &texture );

        Sampler CreateSampler( const rhi::SamplerDescriptor &descriptor );
        Sampler GetOrCreateSampler( const rhi::SamplerDescriptor &descriptor );
        void DestroySampler( Sampler &sampler );

        FramebufferAttachment CreateColorAttachment( const Uint32 width, const Uint32 height );

        FramebufferAttachment CreateDepthAttachment( const Uint32 width, const Uint32 height );

        ConstantBuffer CreateConstantBuffer( const ConstantBufferDescriptor &descriptor );
        void DestroyConstantBuffer( ConstantBuffer &constantBuffer );

        VertexShader CreateVertexShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        VertexShader CreateVertexShader( ShaderAsset::ConstRef shaderAsset );

        VertexShader GetOrCreateVertexShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        VertexShader GetOrCreateVertexShader( ShaderAsset::ConstRef shaderAsset );

        void DestroyVertexShader( VertexShader &vertexShader );

        PixelShader CreatePixelShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        PixelShader CreatePixelShader( ShaderAsset::ConstRef shaderAsset );

        PixelShader GetOrCreatePixelShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        PixelShader GetOrCreatePixelShader( ShaderAsset::ConstRef shaderAsset );

        void DestroyPixelShader( PixelShader &pixelShader );

        Framebuffer CreateFramebuffer( std::initializer_list< FramebufferAttachment > colorAttachments,
            const FramebufferAttachment &depthAttachment );

        Framebuffer CreateFramebuffer(
            const primitive::FixedVector< FramebufferAttachment, rhi::s_MaxRenderTargets > &colorAttachments,
            const FramebufferAttachment &depthAttachment );

        void ResizeFramebuffer( Framebuffer &framebuffer, const Uint32 width, const Uint32 height );

        void DestroyFramebuffer( Framebuffer &framebuffer );

        BindingLayout CreateBindingLayout( const rhi::BindingLayout &layout );
        void DestroyBindingLayout( BindingLayout &bindingLayout );

        BindingSet CreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
            const BindingLayout &layout,
            foundation::Flags< rhi::ShaderStage > shaderStage );

        void DestroyBindingSet( BindingSet &bindingSet );

        void CreateBindingSetAndLayout( const rhi::BindingSetDescriptor &descriptor,
            foundation::Flags< rhi::ShaderStage > shaderStage,
            BindingLayout &layout,
            BindingSet &set );

        GraphicsPipeline CreateGraphicsPipeline( const GraphicsPipelineDescriptor &descriptor );
        void DestroyGraphicsPipeline( GraphicsPipeline &pipeline );

        rhi::Object GetShaderResourceView( const Texture &texture );

        FramebufferAttachmentSet &GetFramebufferAttachmentSet( const Framebuffer &framebuffer );
        const FramebufferAttachmentSet &GetFramebufferAttachmentSet( const Framebuffer &framebuffer ) const;

        struct ShaderKey final
        {
            ShaderKey( const primitive::Vector< Byte > &byteCode,
                const primitive::String &EntryPoint,
                const primitive::String &targetProfile )
            {
                Hash = std::hash< primitive::Vector< Byte > >{}( byteCode );
                Hash = foundation::HashCombine( Hash, std::hash< primitive::String >{}( EntryPoint ) );
                Hash = foundation::HashCombine( Hash, std::hash< primitive::String >{}( targetProfile ) );
            }

            bool operator==( const ShaderKey &other ) const noexcept
            {
                return Hash == other.Hash;
            }

            bool operator!=( const ShaderKey &other ) const noexcept
            {
                return Hash != other.Hash;
            }

            foundation::HashCode Hash;
        };

      private:
        rhi::GraphicsDevice &m_Device;

        primitive::Vector< VertexBuffer > m_VertexBuffers;
        primitive::Vector< IndexBuffer > m_IndexBuffers;
        primitive::Vector< Texture > m_Textures;
        primitive::Vector< Sampler > m_Samplers;
        primitive::Vector< ConstantBuffer > m_ConstantBuffers;
        primitive::Vector< VertexShader > m_VertexShaders;
        primitive::Vector< PixelShader > m_PixelShaders;
        primitive::Vector< Framebuffer > m_Framebuffers;
        primitive::Vector< BindingLayout > m_BindingLayouts;
        primitive::Vector< BindingSet > m_BindingSets;
        primitive::Vector< GraphicsPipeline > m_GraphicsPipelines;

        primitive::HashMap< TextureAsset::ConstRef, Texture > m_TextureCache;

        primitive::HashMap< ShaderKey, VertexShader > m_VertexShaderCache;
        primitive::HashMap< ShaderKey, PixelShader > m_PixelShaderCache;
        primitive::HashMap< rhi::SamplerDescriptor, Sampler > m_SamplerCache;

        rhi::GPUBufferHandleManager m_GPUBufferHandleManager;
        rhi::TextureHandleManager m_TextureHandleManager;
        rhi::SamplerHandlerManager m_SamplerHandleManager;
        rhi::FramebufferHandleManager m_FramebufferHandleManager;
        rhi::ShaderHandleManager m_ShaderHandleManager;
        rhi::BindingLayoutHandleManager m_BindingLayoutHandleManager;
        rhi::BindingSetHandleManager m_BindingSetHandleManager;
        rhi::GraphicsPipelineHandleManager m_GraphicsPipelineHandleManager;

        FramebufferAttachmentSetHandleManager m_FramebufferAttachmentSetHandleManager;
        primitive::Array< FramebufferAttachmentSet, rhi::s_MaxRenderTargets > m_FramebufferAttachmentSets;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::ResourceManager::ShaderKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::ResourceManager::ShaderKey &key ) const
        {
            return key.Hash;
        }
    };
}