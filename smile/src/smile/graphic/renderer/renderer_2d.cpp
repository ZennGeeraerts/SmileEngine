/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer_2d.h"

#include "smile/common/primitive/collection/hash_map.h"

#include "render_engine.h"
#include "resource/resource_manager.h"
#include "shader/shader_asset.h"

namespace smile::graphic
{
    struct Renderer2DStorage final
    {
        GraphicsPipeline::Ref pPipeline;
        VertexBuffer::Ref pQuadVertexBuffer;
        IndexBuffer::Ref pQuadIndexBuffer;
        ConstantBuffer::Ref pCameraCB;
        ConstantBuffer::Ref pPerObjectCB;
        ConstantBuffer::Ref pMaterialCB;
        Sampler::Ref pSampler;

        BindingSet::Ref pVertexShaderBindingSet;
        primitive::HashMap< Texture::Ref, BindingSet::Ref > pPixelShaderBindingSetsWithTexture;
        BindingSet::Ref pPixelShaderBindingSetNoTexture;
    };

    struct Renderer2DMaterial final
    {
        DirectX::XMFLOAT4 Color;
        bool UseTexture;
    };

    static Renderer2DStorage *s_pStorage;

    void Renderer2D::Initialize()
    {
        s_pStorage = new Renderer2DStorage{};

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();
        const auto &shaderLibrary = RenderEngine::GetShaderLibrary();

        rhi::BufferLayout inputLayout{
            { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RG32_FLOAT, "TEXCOORD" } };

        {
            GraphicsPipelineDescriptor psoDesc{};
            psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
            psoDesc.InputLayout = inputLayout;
            psoDesc.pVertexShader = shaderLibrary.GetShader( "PosColTex.vs" )->GetVertexShader();
            psoDesc.pPixelShader = shaderLibrary.GetShader( "PosColTex.ps" )->GetPixelShader();

            auto vertexShaderBindingLayout = rhi::BindingLayout{ { rhi::ShaderStage::Vertex } };
            vertexShaderBindingLayout.AddElement( { 0, rhi::ResourceType::ConstantBuffer } );
            vertexShaderBindingLayout.AddElement( { 1, rhi::ResourceType::ConstantBuffer } );
            psoDesc.BindingLayouts.PushBack( std::move( vertexShaderBindingLayout ) );

            auto pixelShaderBindingLayout = rhi::BindingLayout{ { rhi::ShaderStage::Pixel } };
            pixelShaderBindingLayout.AddElement( { 0, rhi::ResourceType::ConstantBuffer } );
            pixelShaderBindingLayout.AddElement( { 0, rhi::ResourceType::Texture_SRV } );
            pixelShaderBindingLayout.AddElement( { 0, rhi::ResourceType::Sampler } );
            psoDesc.BindingLayouts.PushBack( std::move( pixelShaderBindingLayout ) );

            s_pStorage->pPipeline = resourceManager.CreateGraphicsPipeline( psoDesc );
        }

        {
            const Count quadVerticesCount = 12;
            float quadVertices[] = { -0.5f,
                -0.5f,
                0.0f,
                -1,
                -1,
                /*1*/ -0.5f,
                0.5f,
                0.0f,
                -1,
                1,
                /*2*/ 0.5f,
                -0.5f,
                0.0f,
                1,
                -1 /*3*/,
                0.5f,
                0.5f,
                0.0f,
                1,
                1 /*4*/ };

            s_pStorage->pQuadVertexBuffer =
                resourceManager.CreateVertexBuffer( quadVertices, quadVerticesCount, inputLayout );
        }

        {
            const Uint32 quadIndicesCount = 6;
            Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };
            s_pStorage->pQuadIndexBuffer = resourceManager.CreateIndexBuffer( quadIndices, quadIndicesCount );
        }

        {
            rhi::BindingSetDescriptor vertexShaderBindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, s_pStorage->pPerObjectCB->GetHandle() ) },
                { rhi::BindingSetElement::CreateConstantBuffer( 1, s_pStorage->pCameraCB->GetHandle() ) } };

            s_pStorage->pVertexShaderBindingSet =
                resourceManager.CreateBindingSet( vertexShaderBindingSetDesc, { rhi::ShaderStage::Vertex } );

            rhi::BindingSetDescriptor pixelShaderBindingSetNoTextureDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, s_pStorage->pMaterialCB->GetHandle() ) },
                { rhi::BindingSetElement::CreateSampler( 0, s_pStorage->pSampler->GetHandle() ) } };

            s_pStorage->pPixelShaderBindingSetNoTexture =
                resourceManager.CreateBindingSet( pixelShaderBindingSetNoTextureDesc, { rhi::ShaderStage::Pixel } );
        }
    }

    void Renderer2D::ShutDown()
    {
        delete s_pStorage;
    }

    void Renderer2D::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMFLOAT4X4 viewProjectionMatrix;
        DirectX::XMStoreFloat4x4( &viewProjectionMatrix, viewProjectionMatrixMat );

        s_pStorage->pCameraCB->Initialize( &viewProjectionMatrix );
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::OnRender()
    {
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT2 &position,
        float rotation,
        const DirectX::XMFLOAT2 &size,
        const DirectX::XMFLOAT4 &color )
    {
        DirectX::XMMATRIX worldTransformMat = DirectX::XMMatrixScaling( size.x, size.y, 1 ) *
                                              DirectX::XMMatrixRotationRollPitchYaw( 0, 0, rotation ) *
                                              DirectX::XMMatrixTranslation( position.x, position.y, 0 );
        DirectX::XMFLOAT4X4 worldTransform{};
        DirectX::XMStoreFloat4x4( &worldTransform, worldTransformMat );

        DrawQuad( worldTransform, color );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform,
        const ecs::SpriteRendererComponent &spriteRendererComponent )
    {
        if ( spriteRendererComponent.pTexture )
            DrawQuad( worldTransform, spriteRendererComponent.pTexture, spriteRendererComponent.Color );
        else
            DrawQuad( worldTransform, spriteRendererComponent.Color );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform, const DirectX::XMFLOAT4 &color )
    {
        GraphicsState state{};

        state.pPipeline = s_pStorage->pPipeline;
        // TODO: Set framebuffer

        VertexBufferBinding vertexBufferBinding{ s_pStorage->pQuadVertexBuffer, 0, 0 };
        state.VertexBuffers.PushBack( std::move( vertexBufferBinding ) );

        state.IndexBuffer = IndexBufferBinding{ s_pStorage->pQuadIndexBuffer, rhi::Format::R32_UINT, 0 };

        state.pBindings.PushBack( s_pStorage->pVertexShaderBindingSet );
        state.pBindings.PushBack( s_pStorage->pPixelShaderBindingSetNoTexture );

        s_pStorage->pPerObjectCB->Initialize( &worldTransform );

        const Renderer2DMaterial material{ color, false };
        s_pStorage->pMaterialCB->Initialize( &material );

        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();
        renderSystem.SetGraphicsState( state );
        renderSystem.DrawIndexed( s_pStorage->pQuadIndexBuffer->GetIndexCount() );
    }

    void Renderer2D::DrawQuad( const DirectX::XMFLOAT4X4 &worldTransform,
        Texture::ConstRef pTexture,
        const DirectX::XMFLOAT4 &color )
    {
        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        GraphicsState state{};

        state.pPipeline = s_pStorage->pPipeline;
        // TODO: Set framebuffer

        VertexBufferBinding vertexBufferBinding{ s_pStorage->pQuadVertexBuffer, 0, 0 };
        state.VertexBuffers.PushBack( std::move( vertexBufferBinding ) );

        state.IndexBuffer = IndexBufferBinding{ s_pStorage->pQuadIndexBuffer, rhi::Format::R32_UINT, 0 };

        state.pBindings.PushBack( s_pStorage->pVertexShaderBindingSet );

        BindingSet::Ref pPixelShaderBindingSet = [&]
        {
            if ( s_pStorage->pPixelShaderBindingSetsWithTexture.HasItemAtKey( pTexture ) )
            {
                return s_pStorage->pPixelShaderBindingSetsWithTexture.GetItemAtKey( pTexture );
            }
            else
            {
                rhi::BindingSetDescriptor bindingSetDesc{
                    { rhi::BindingSetElement::CreateConstantBuffer( 0, s_pStorage->pMaterialCB->GetHandle() ) },
                    { rhi::BindingSetElement::CreateTextureSRV( 0, pTexture->GetHandle(), pTexture->GetFormat() ) },
                    { rhi::BindingSetElement::CreateSampler( 0, s_pStorage->pSampler->GetHandle() ) } };

                BindingSet::Ref pBindingSet =
                    renderSystem.GetResourceManager().CreateBindingSet( bindingSetDesc, { rhi::ShaderStage::Pixel } );

                return pBindingSet;
            }
        }();

        state.pBindings.PushBack( pPixelShaderBindingSet );

        s_pStorage->pPerObjectCB->Initialize( &worldTransform );

        const Renderer2DMaterial material{ color, true };
        s_pStorage->pMaterialCB->Initialize( &material );

        renderSystem.SetGraphicsState( state );
        renderSystem.DrawIndexed( s_pStorage->pQuadIndexBuffer->GetIndexCount() );
    }
}