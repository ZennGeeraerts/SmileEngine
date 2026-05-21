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
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/sprite/renderer_2d.h"

namespace smile::graphic
{
    void SpriteTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();

        m_RenderEngine = RenderEngine::Create( rhi::RendererBackendType::D3D11 );
        m_SwapChain = m_RenderEngine->CreateSwapChain( &window );
        m_Renderer = m_RenderEngine->CreateRenderer();
        m_Scene = m_RenderEngine->CreateScene( m_SwapChain );

        auto &resourceManager = m_RenderEngine->GetResourceManager();
        auto &materialSystem = m_RenderEngine->GetMaterialSystem();

        Renderer2D::GetInstance().Initialize( resourceManager, m_RenderEngine->GetShaderLibrary() );

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

        m_Scene->GetView().SetViewProjectionMatrix( viewMatrix, projectionMatrix );

        TextureAsset::Ref textureAsset = m_RenderEngine->GetTextureManager().Load( "resources/textures/uv_grid.png" );

        {
            MaterialLayout layout{};
            layout.Parameters.EmplaceBack( "Color", MaterialParameterType::Float3, 0u, 12u );
            layout.Parameters.EmplaceBack( "UseTexture", MaterialParameterType::Int, 12u, 4u );

            layout.Textures.EmplaceBack( "Diffuse", 0u );

            layout.CbSlot = 2;
            layout.CbSize = 16u;
            layout.Visibility = { rhi::ShaderStage::Pixel };

            auto &shaderLibrary = m_RenderEngine->GetShaderLibrary();
            auto vertexShader = shaderLibrary.Get( "pos_tex.vs" );
            auto pixelShader = shaderLibrary.Get( "col_tex.ps" );

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
            desc.TextureBindings["Diffuse"] = { resourceManager.GetOrCreateTexture2D( textureAsset ), samplerDesc };

            m_Material = materialSystem.CreateMaterial( "DefaultSprite", layout, desc );
        }
    }

    void SpriteTestLayer::OnDetach()
    {
        Renderer2D::GetInstance().ShutDown();
    }

    void SpriteTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        auto &renderer2D = Renderer2D::GetInstance();

        m_Scene->Clear();
        renderer2D.BeginFrame( *m_Scene );

        DirectX::XMFLOAT4X4 worldTransform;
        DirectX::XMStoreFloat4x4( &worldTransform, DirectX::XMMatrixIdentity() );

        renderer2D.DrawSprite( worldTransform, m_Material.GetDefaultInstance() );

        m_Scene->GetView().OnUpdate();

        const auto &backBuffer = m_RenderEngine->GetRenderTarget( m_SwapChain );
        m_Renderer->BeginFrame();
        m_Renderer->OnRender( *m_Scene );
        m_Renderer->EndFrame( *m_SwapChain );
    }

    void SpriteTestLayer::OnEvent( window::Event &event )
    {
    }

    void SpriteTestLayer::OnImGuiRender()
    {
    }
}