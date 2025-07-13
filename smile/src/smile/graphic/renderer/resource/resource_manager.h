/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"

#include "smile/graphic/resource/image.h"

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "frame_buffer.h"
#include "graphics_pipeline.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/vertex_shader.h"
#include "smile/graphic/renderer/shader/pixel_shader.h"
#include "smile/graphic/renderer/shader/binding_set.h"

#include "smile/graphic/rhi/resource/frame_buffer.h"

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

        VertexBuffer::Ref
        CreateVertexBuffer( void *pVertices, const Count vertexCount, const rhi::BufferLayout &layout );
        VertexBuffer::Ref CreateDynamicVertexBuffer( const Count vertexCount, const rhi::BufferLayout &layout );

        IndexBuffer::Ref CreateIndexBuffer( Uint32 *pIndices, const Count indexCount );

        Texture::Ref CreateTexture2D( Image::ConstRef pImage, bool updateable );
        Texture::Ref CreateTextureCube( Image::ConstRef pImage, bool updateable );

        ConstantBuffer::Ref CreateConstantBuffer( const ConstantBufferDescriptor &descriptor );

        VertexShader::Ref CreateVertexShader( const std::vector< Byte > &byteCode,
            const std::string &entryPoint,
            const std::string &targetProfile );

        PixelShader::Ref CreatePixelShader( const std::vector< Byte > &byteCode,
            const std::string &entryPoint,
            const std::string &targetProfile );

        Framebuffer::Ref CreateFramebuffer( std::initializer_list< FramebufferAttachment > colorAttachments,
            const FramebufferAttachment &depthAttachment );

        void ResizeFramebuffer( memory::Ref< Framebuffer > pFramebuffer, Uint32 width, Uint32 height );

        BindingSet::Ref CreateBindingSet( const rhi::BindingSetDescriptor &descriptor,
            const rhi::BindingLayout &layout );

        GraphicsPipeline::Ref CreateGraphicsPipeline( rhi::PrimitiveTopology topology,
            const rhi::BufferLayout &inputLayout,
            const rhi::RenderState &renderState,
            VertexShader::ConstRef pVertexShader,
            PixelShader::ConstRef pPixelShader,
            const primitive::Vector< rhi::BindingLayout > &bindingLayouts );

      private:
        rhi::GraphicsDevice *m_pDevice = nullptr;
        primitive::Vector< VertexBuffer::Ref > m_pVertexBuffers;
        primitive::Vector< IndexBuffer::Ref > m_pIndexBuffers;
        primitive::Vector< Texture::Ref > m_pTextures;
        primitive::Vector< ConstantBuffer::Ref > m_pConstantBuffers;
        primitive::Vector< VertexShader::Ref > m_pVertexShaders;
        primitive::Vector< PixelShader::Ref > m_pPixelShaders;
        primitive::Vector< Framebuffer::Ref > m_pFramebuffers;
        primitive::Vector< BindingSet::Ref > m_pBindingSets;
        primitive::Vector< GraphicsPipeline::Ref > m_pGraphicsPipelines;

        rhi::GPUBufferHandleManager m_GPUBufferHandleManager;
        rhi::TextureHandleManager m_TextureHandleManager;
        rhi::FramebufferHandleManager m_FramebufferHandleManager;
        rhi::ShaderHandleManager m_ShaderHandleManager;
        rhi::BindingSetHandleManager m_BindingSetHandleManager;
        rhi::GraphicsPipelineHandleManager m_GraphicsPipelineHandleManager;
    };
}