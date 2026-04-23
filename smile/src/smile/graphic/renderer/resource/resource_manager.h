/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"

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
#include "smile/graphic/renderer/shader/binding_set.h"

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
        ResourceManager() = default;
        ~ResourceManager();

        void Initialize( rhi::GraphicsDevice *pDevice );

        VertexBuffer CreateVertexBuffer( void *pVertices, const Count vertexCount, const rhi::BufferLayout &layout );

        VertexBuffer CreateDynamicVertexBuffer( const Count vertexCount, const rhi::BufferLayout &layout );

        IndexBuffer CreateIndexBuffer( Uint32 *pIndices, const Count indexCount );

        Texture CreateTexture2D( Image::ConstRef pImage, bool updateable );

        Texture CreateTextureCube( Image::ConstRef pImage, bool updateable );

        Texture
        CreateTextureFromNative( rhi::Object nativeTexture, rhi::ObjectType type, const rhi::TextureDescriptor &desc );

        Sampler CreateSampler( const rhi::SamplerDescriptor &descriptor );
        Sampler GetOrCreateSampler( const rhi::SamplerDescriptor &descriptor );

        FramebufferAttachment CreateColorAttachment( const Uint32 width, const Uint32 height );

        FramebufferAttachment CreateDepthAttachment( const Uint32 width, const Uint32 height );

        ConstantBuffer CreateConstantBuffer( const ConstantBufferDescriptor &descriptor );

        VertexShader CreateVertexShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        VertexShader CreateVertexShader( ShaderAsset::ConstRef shaderAsset );

        VertexShader GetOrCreateVertexShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        VertexShader GetOrCreateVertexShader( ShaderAsset::ConstRef shaderAsset );

        PixelShader CreatePixelShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        PixelShader CreatePixelShader( ShaderAsset::ConstRef shaderAsset );

        PixelShader GetOrCreatePixelShader( const primitive::Vector< Byte > &byteCode,
            const primitive::String &entryPoint,
            const primitive::String &targetProfile );

        PixelShader GetOrCreatePixelShader( ShaderAsset::ConstRef shaderAsset );

        Framebuffer CreateFramebuffer( std::initializer_list< FramebufferAttachment > colorAttachments,
            const FramebufferAttachment &depthAttachment );

        Framebuffer CreateFramebuffer( const primitive::Vector< FramebufferAttachment > &colorAttachments,
            const FramebufferAttachment &depthAttachment );

        void ResizeFramebuffer( Framebuffer &framebuffer, const Uint32 width, const Uint32 height );

        BindingSet CreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
            foundation::Flags< rhi::ShaderStage > shaderStage );

        GraphicsPipeline CreateGraphicsPipeline( const GraphicsPipelineDescriptor &descriptor );

        rhi::Object GetShaderResourceView( const Texture &texture );

        struct ShaderKey final
        {
            ShaderKey( const primitive::Vector< Byte > &byteCode,
                const primitive::String &EntryPoint,
                const primitive::String &targetProfile )
                : ByteCode{ byteCode }, EntryPoint{ EntryPoint }, TargetProfile{ targetProfile }
            {
            }

            bool operator==( const ShaderKey &other ) const
            {
                return primitive::array::IsEqual( ByteCode, other.ByteCode ) && EntryPoint == other.EntryPoint &&
                       TargetProfile == other.TargetProfile;
            }

            bool operator!=( const ShaderKey &other ) const
            {
                return !( *this == other );
            }

            foundation::HashCode GetHashCode() const
            {
                foundation::HashCode hash = std::hash< primitive::Vector< Byte > >{}( ByteCode );
                hash = foundation::HashCombine( hash, std::hash< primitive::String >{}( EntryPoint ) );
                hash = foundation::HashCombine( hash, std::hash< primitive::String >{}( TargetProfile ) );
                return hash;
            }

            primitive::Vector< Byte > ByteCode;
            primitive::String EntryPoint;
            primitive::String TargetProfile;
        };

      private:
        rhi::GraphicsDevice *m_pDevice = nullptr;

        primitive::Vector< VertexBuffer > m_VertexBuffers;
        primitive::Vector< IndexBuffer > m_IndexBuffers;
        primitive::Vector< Texture > m_Textures;
        primitive::Vector< Sampler > m_Samplers;
        primitive::Vector< ConstantBuffer > m_ConstantBuffers;
        primitive::Vector< VertexShader > m_VertexShaders;
        primitive::Vector< PixelShader > m_PixelShaders;
        primitive::Vector< Framebuffer > m_Framebuffers;
        primitive::Vector< BindingSet > m_BindingSets;
        primitive::Vector< GraphicsPipeline > m_GraphicsPipelines;

        primitive::HashMap< ShaderKey, VertexShader > m_VertexShaderCache;
        primitive::HashMap< ShaderKey, PixelShader > m_PixelShaderCache;
        primitive::HashMap< rhi::SamplerDescriptor, Sampler > m_SamplerCache;

        rhi::GPUBufferHandleManager m_GPUBufferHandleManager;
        rhi::TextureHandleManager m_TextureHandleManager;
        rhi::SamplerHandlerManager m_SamplerHandleManager;
        rhi::FramebufferHandleManager m_FramebufferHandleManager;
        rhi::ShaderHandleManager m_ShaderHandleManager;
        rhi::BindingSetHandleManager m_BindingSetHandleManager;
        rhi::GraphicsPipelineHandleManager m_GraphicsPipelineHandleManager;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::ResourceManager::ShaderKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::ResourceManager::ShaderKey &key ) const
        {
            return key.GetHashCode();
        }
    };
}