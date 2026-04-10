/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        sprite_test_layer.cpp
 * @author      Zenn Geeraerts
 * @created     31 March 2026
 * @brief       Sprite test application layer
 */
#include "sprite_test_layer.h"

#include "smile/core/application/application.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/forward_renderer.h"
#include "smile/graphic/sprite/renderer_2d.h"
#include "smile/graphic/sprite/texture_manager.h"

namespace smile::graphic
{
    void SpriteTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();
        RenderEngine::Initialize( &window );
        Renderer2D::GetInstance().Initialize();

        DirectX::XMFLOAT4X4 viewMatrix{};
        {
            DirectX::XMMATRIX transformMat = DirectX::XMMatrixScaling( 1.0f, 1.0f, 1.0f ) *
                                             DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, 0.0f ) *
                                             DirectX::XMMatrixTranslation( 0.0f, 2.0f, -10.0f );

            DirectX::XMStoreFloat4x4( &viewMatrix, DirectX::XMMatrixInverse( nullptr, transformMat ) );
        }

        DirectX::XMFLOAT4X4 projectionMatrix{};
        {
            auto projectionMatrixMat =
                DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( 30.f ), 1.778f, 0.1f, 2500.f );

            DirectX::XMStoreFloat4x4( &projectionMatrix, projectionMatrixMat );
        }

        m_View.SetViewProjectionMatrix( viewMatrix, projectionMatrix );

        TextureManager::CreateInstance();
        TextureAsset::Ref textureAsset = TextureManager::GetInstance().GetTexture( "resources/textures/uv_grid.png" );
        auto &resourceManager = RenderEngine::GetRenderContext().GetResourceManager();

        {
            MaterialLayout layout{};
            layout.Parameters.EmplaceBack( "Color", MaterialParameterType::Float3, 0u, 12u );
            layout.Parameters.EmplaceBack( "UseTexture", MaterialParameterType::Int, 12u, 4u );

            layout.Textures.EmplaceBack( "Diffuse", 0u );

            layout.CbSlot = 2;
            layout.CbSize = 16u;
            layout.Visibility = { rhi::ShaderStage::Pixel };

            auto &shaderLibrary = RenderEngine::GetShaderLibrary();
            auto vertexShader = shaderLibrary.GetShader( "pos_tex.vs" );
            auto pixelShader = shaderLibrary.GetShader( "col_tex.ps" );

            auto program = Program::Create( vertexShader, pixelShader );

            rhi::SamplerDescriptor samplerDesc{};
            samplerDesc.Filtering = rhi::SamplerFiltering::MinMagMipLinear;
            samplerDesc.AddressingU = rhi::SamplerAddressing::Wrap;
            samplerDesc.AddressingV = rhi::SamplerAddressing::Wrap;
            samplerDesc.AddressingW = rhi::SamplerAddressing::Wrap;

            MaterialDescriptor desc{};
            desc.ShaderProgram = program;
            desc.Parameters["Color"] = DirectX::XMFLOAT3{ 1.0f, 0.0f, 0.0f };
            desc.Parameters["UseTexture"] = 1;
            desc.TextureBindings["Diffuse"] = { textureAsset->GetTexture(), samplerDesc };

            m_Material = RenderEngine::GetMaterialSystem().CreateMaterial( "DefaultSprite", layout, desc );
        }
    }

    void SpriteTestLayer::OnDetach()
    {
        Renderer2D::GetInstance().ShutDown();
        RenderEngine::ShutDown();
    }

    void SpriteTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        auto &renderContext = RenderEngine::GetRenderContext();
        auto &renderer2D = Renderer2D::GetInstance();

        DirectX::XMFLOAT4X4 worldTransform;
        DirectX::XMStoreFloat4x4( &worldTransform, DirectX::XMMatrixIdentity() );

        renderer2D.DrawSprite( worldTransform, m_Material->GetDefaultInstance() );

        m_View.OnUpdate();

        renderContext.Clear(
            renderContext.GetBackBuffer(), math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        renderContext.BeginFrame();

        auto &forwardRenderer = ForwardRenderer::GetInstance();
        {
            forwardRenderer.BeginScene( m_View );
            forwardRenderer.OnRender( renderContext.GetBackBuffer() );
            forwardRenderer.EndScene();
        }

        renderContext.EndFrame();
    }

    void SpriteTestLayer::OnEvent( window::Event &event )
    {
    }

    void SpriteTestLayer::OnImGuiRender()
    {
    }
}