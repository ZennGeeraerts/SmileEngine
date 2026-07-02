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
 * @file        renderer_test_layer.cpp
 * @author      Zenn Geeraerts
 * @created     10 March 2026
 * @brief       Renderer test application layer
 */
#include "renderer_test_layer.h"

#include "smile/core/application/application.h"
#include "smile/core/world/ecs/transform_component.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/renderable.h"
#include "smile/graphic/renderer/scene/ecs/camera_component.h"
#include "smile/graphic/renderer/mesh/ecs/mesh_renderer_component.h"

namespace smile::graphic
{
    void RendererTestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();

        m_RenderEngine = RenderEngine::Create( rhi::RendererBackendType::D3D11 );
        m_SwapChain = &m_RenderEngine->CreateSwapChain( &window );
        m_Renderer = &m_RenderEngine->CreateRenderer();

        const auto &shaderLibrary = m_RenderEngine->GetShaderLibrary();

        m_RenderWorld = &m_RenderEngine->CreateWorld();

        SceneCamera camera;
        camera.SetViewportSize( window.GetWidth(), window.GetHeight() );
        camera.SetPerspectiveCamera( 30.f, 0.1f, 2500.f );

        auto cameraEntity = m_RenderWorld->CreateEntity();
        m_MeshEntity = m_RenderWorld->CreateEntity();

        auto &cameraComponent = m_RenderWorld->AddComponent< ecs::CameraComponent >( cameraEntity );
        cameraComponent.Camera = camera;
        cameraComponent.IsPrimary = true;
        cameraComponent.HasFixedAspectRatio = false;
        cameraComponent.RenderTarget = m_RenderEngine->GetRenderTarget( *m_SwapChain );

        auto &cameraTransform = m_RenderWorld->GetComponent< world::ecs::TransformComponent >( cameraEntity );
        cameraTransform.WorldTranslation = DirectX::XMFLOAT3{ 0.0f, 1.0f, -5.0f };

        MeshSource::Ref mesh = memory::CreateRef< MeshSource >();
        mesh->SetVertexCount( 4 );
        mesh->AddPosition( { -0.5f, -0.5f, 0.0f } );
        mesh->AddPosition( { -0.5f, 0.5f, 0.0f } );
        mesh->AddPosition( { 0.5f, -0.5f, 0.0f } );
        mesh->AddPosition( { 0.5f, 0.5f, 0.0f } );
        mesh->AddTexCoord( { 0.0f, 1.0f } );
        mesh->AddTexCoord( { 0.0f, 0.0f } );
        mesh->AddTexCoord( { 1.0f, 1.0f } );
        mesh->AddTexCoord( { 1.0f, 0.0f } );
        mesh->SetIndexCount( 6 );
        mesh->AddIndex( 0, 0 );
        mesh->AddIndex( 1, 1 );
        mesh->AddIndex( 2, 2 );
        mesh->AddIndex( 3, 2 );
        mesh->AddIndex( 4, 1 );
        mesh->AddIndex( 5, 3 );

        auto &materialManager = m_RenderEngine->GetMaterialManager();
        auto &materialInstanceManager = m_RenderEngine->GetMaterialInstanceManager();

        MaterialAsset::Ref materialAsset = materialManager.CreateMaterial(
            "DefaultMeshMaterial", shaderLibrary.Get( "pos_tex.vs" ), shaderLibrary.Get( "col_tex.ps" ) );

        MaterialInstanceAsset::Ref materialInstance = materialInstanceManager.CreateMaterialInstance(
            "DefaultMeshMaterialInstance", materialAsset, materialAsset->GetDescriptor() );
        materialInstance->SetParameter( "Color", DirectX::XMFLOAT3{ 0.0f, 1.0f, 0.0f } );
        materialInstance->SetParameter( "UseTexture", 0u );

        auto &meshRendererComponent = m_RenderWorld->AddComponent< ecs::MeshRendererComponent >( m_MeshEntity );
        meshRendererComponent.Mesh = mesh;
        meshRendererComponent.Material = materialInstance;

        auto &meshTransform = m_RenderWorld->GetComponent< world::ecs::TransformComponent >( m_MeshEntity );
        meshTransform.WorldTranslation = DirectX::XMFLOAT3{ 1.0f, 1.0f, 0.0f };
    }

    void RendererTestLayer::OnDetach()
    {
    }

    void RendererTestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        m_AccumulatedTime += deltaTime;
        if ( m_AccumulatedTime >= m_ToggleTextureTime )
        {
            m_AccumulatedTime = 0.0f;

            auto &materialInstance = m_RenderWorld->GetComponent< MaterialInstance >( m_MeshEntity );

            Uint32 useTexture = std::get< Uint32 >( materialInstance.GetParameter( "UseTexture" ) );
            useTexture = ( useTexture == 0 ) ? 1u : 0u;
            materialInstance.SetParameter( "UseTexture", useTexture );
        }

        auto &meshTransform = m_RenderWorld->GetComponent< world::ecs::TransformComponent >( m_MeshEntity );
        meshTransform.WorldRotation.x += deltaTime * 0.25f;
        meshTransform.WorldRotation.y += deltaTime * 0.25f;

        m_Renderer->BeginFrame();
        m_Renderer->OnRender( *m_RenderWorld );
        m_Renderer->EndFrame( *m_SwapChain );
    }

    void RendererTestLayer::OnEvent( window::Event &event )
    {
    }

    void RendererTestLayer::OnImGuiRender()
    {
    }
}