/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile_editor_layer.h"

#include "smile/core/world/world_manager.h"
#include "smile/core/world/ecs/transform_system.h"
#include "smile/core/window/file_dialog.h"
#include "smile/core/ecs/state/system_factory.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/animation/ecs/animation_system.h"
#include "smile/graphic/camera/ecs/camera_system.h"
#include "smile/graphic/ecs/graphic_system.h"

#include "smile/physics/physics_engine.h"
#include "smile/physics/ecs/physics_system.h"

#include "smile/scripting/ecs/script_system.h"

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

namespace smile
{
    /*-----------------------------------------------------------------------------------------------------------*/
    /*---------------------------------------------- Editor Layer -----------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------------*/

    SmileEditorLayer::SmileEditorLayer() : application::Layer( "SmileEditorLayer" )
    {
    }

    void SmileEditorLayer::OnAttach()
    {
        smile::ecs::state::SystemFactory::RegisterSystem< scripting::ecs::ScriptSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< world::ecs::TransformSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< physics::ecs::PhysicsSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::AnimationSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::CameraSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::GraphicSystem >();

        graphic::RenderEngine::GetRenderSystem().SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
            DirectX::Colors::DodgerBlue.f[1],
            DirectX::Colors::DodgerBlue.f[2],
            DirectX::Colors::DodgerBlue.f[3] } );

        m_EditorCamera = graphic::EditorCamera{ 30.f, 1.778f, 0.1f, 2500.f };

        // Icon
        auto &resourceManager = graphic::RenderEngine::GetRenderSystem().GetResourceManager();
        m_pIconPlay = resourceManager.CreateTexture2D( "resources/icons/play_button.png" );
        m_pIconSimulate = resourceManager.CreateTexture2D( "resources/icons/simulate_button.png" );
        m_pIconStop = resourceManager.CreateTexture2D( "resources/icons/stop_button.png" );

        auto commandLineArgs = application::Application::GetInstance().GetDescriptor().CommandLineArgs;
        if ( commandLineArgs.Count > 1 )
        {
            auto projectFilePath = commandLineArgs[1];
            OpenProject( projectFilePath );
        }
        else
        {
            // TODO: prompt the user to select a directory
            // NewProject();

            if ( !OpenProject() )
                application::Application::GetInstance().ShutDown();
        }

        physics::PhysicsEngine::CreateInstance();

        auto pEditorState = m_pEditorWorld->CreateState( "editor" );
        pEditorState->AddSystem( std::string{ world::ecs::TransformSystem::GetStaticName() } );
        pEditorState->AddOverlaySystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );

        auto pSimulateState = m_pEditorWorld->CreateState( "simulate" );
        pSimulateState->AddSystem( std::string{ world::ecs::TransformSystem::GetStaticName() } );
        pSimulateState->AddSystem( std::string{ physics::ecs::PhysicsSystem::GetStaticName() } );
        pSimulateState->AddOverlaySystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );

        auto pRuntimeState = m_pEditorWorld->CreateState( "runtime" );
        pRuntimeState->AddSystem( std::string{ scripting::ecs::ScriptSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ world::ecs::TransformSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ physics::ecs::PhysicsSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::AnimationSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::CameraSystem::GetStaticName() } );
        pRuntimeState->AddOverlaySystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );

        m_pEditorWorld->ChangeState( "editor" );
    }

    void SmileEditorLayer::OnDetach()
    {
        m_pEditorWorld.reset();
        world::WorldManager::UnloadActive(); // TRICKY: Ref count needs to be 0 after this call, consider not making
                                             // this a ref

        physics::PhysicsEngine::RemoveInstance();
    }

    void SmileEditorLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        memory::Ref< graphic::Scene > pScene = graphic::RenderEngine::GetScene();

        if ( ( !math::AreEqual( m_ViewportSize.x, static_cast< float >( pScene->GetViewportWidth() ) ) ||
                 !math::AreEqual( m_ViewportSize.y, static_cast< float >( pScene->GetViewportHeight() ) ) ) &&
             ( m_ViewportSize.x > 0 ) && ( m_ViewportSize.y > 0 ) )
        {
            m_EditorCamera.SetViewportSize( m_ViewportSize.x, m_ViewportSize.y );
            pScene->OnViewportResize(
                static_cast< Uint32 >( m_ViewportSize.x ), static_cast< Uint32 >( m_ViewportSize.y ) );
        }

        graphic::RenderEngine::GetRenderSystem().Clear();

        if ( ( m_WorldState == WorldState::Edit || m_WorldState == WorldState::Simulate ) && m_IsViewportFocused )
        {
            m_EditorCamera.OnUpdate( deltaTime );
            pScene->SetFallbackCameraData( { &m_EditorCamera, m_EditorCamera.GetTransform() } );
        }

        world::WorldManager::GetActive()->OnUpdate( deltaTime );
    }

    void SmileEditorLayer::OnImGuiRender()
    {
        static bool optFullscreen = true;
        static bool optPadding = false;
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if ( optFullscreen )
        {
            ImGuiViewport *pViewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos( pViewport->GetWorkPos() );
            ImGui::SetNextWindowSize( pViewport->GetWorkSize() );
            ImGui::SetNextWindowViewport( pViewport->ID );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if ( dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode )
            windowFlags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if ( !optPadding )
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );

        static bool dockSpaceOpen = true;
        ImGui::Begin( "DockSpace Demo", &dockSpaceOpen, windowFlags );
        if ( !optPadding )
            ImGui::PopStyleVar();

        if ( optFullscreen )
            ImGui::PopStyleVar( 2 );

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float standardWindowMinSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 350.f;

        if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
        {
            ImGuiID dockspaceID = ImGui::GetID( "MyDockSpace" );
            ImGui::DockSpace( dockspaceID, ImVec2( 0.0f, 0.0f ), dockspaceFlags );
        }

        style.WindowMinSize.x = standardWindowMinSize;

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "Open Project...", "Ctrl+O" ) )
                    OpenProject();

                ImGui::Separator();

                if ( ImGui::MenuItem( "New World", "Ctrl+N" ) )
                    NewWorld();

                if ( ImGui::MenuItem( "Save World", "Ctrl+S" ) )
                    SaveWorld();

                if ( ImGui::MenuItem( "Save World As...", "Ctrl+Shift+S" ) )
                    SaveWorldAs();

                ImGui::Separator();

                if ( ImGui::MenuItem( "Exit" ) )
                    smile::application::Application::GetInstance().ShutDown();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_WorldHierarchyPanel.OnImGuiRender();
        m_pContentBrowserPanel->OnImGuiRender();

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{} );
        ImGui::Begin( "Viewport" );
        m_IsViewportFocused = ImGui::IsWindowFocused();
        m_IsViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image(
            graphic::RenderEngine::GetScene()->GetFinalColor(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y } );

        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( pPayload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                OpenWorld( path );
            }

            ImGui::EndDragDropTarget();
        }

        // Gizmos
        world::Entity selectedEntity = m_WorldHierarchyPanel.GetSelectedEntity();
        if ( selectedEntity && ( m_GizmoType != GizmoType::None ) && ( m_WorldState == WorldState::Edit ) )
        {
            ImGuizmo::SetOrthographic( false );
            ImGuizmo::SetDrawlist();
            float windowWidth = static_cast< float >( ImGui::GetWindowWidth() );
            float windowHeight = static_cast< float >( ImGui::GetWindowHeight() );
            ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight );

            //// Runtime camera
            // auto cameraEntity = m_pActiveWorld->GetPrimaryCameraEntity();
            // if (cameraEntity)
            // {
            // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;

            //// Camera
            // DirectX::XMFLOAT4X4 cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
            // DirectX::XMMATRIX cameraTransformMat = DirectX::XMLoadFloat4x4(&cameraTransform);

            // const DirectX::XMFLOAT4X4& cameraProjectionMatrix = camera.GetProjectionMatrix();
            // DirectX::XMMATRIX cameraViewMatrixMat = DirectX::XMMatrixInverse(nullptr, cameraTransformMat);

            // Editor camera
            const DirectX::XMFLOAT4X4 &cameraProjectionMatrix = m_EditorCamera.GetProjectionMatrix();
            const DirectX::XMFLOAT4X4 &cameraViewMatrix = m_EditorCamera.GetViewMatrix();

            // Entity
            auto &entityTransformComponent = selectedEntity.GetComponent< world::ecs::TransformComponent >();
            auto entityTransform = entityTransformComponent.GetWorldTransform();
            const auto entityTranslation = entityTransformComponent.WorldTranslation;
            const auto entityRotation = entityTransformComponent.WorldRotation;
            const auto entityScale = entityTransformComponent.WorldScale;

            const auto entityRotationDeg =
                DirectX::XMFLOAT3{ DirectX::XMConvertToDegrees( entityTransformComponent.WorldRotation.x ),
                    DirectX::XMConvertToDegrees( entityTransformComponent.WorldRotation.y ),
                    DirectX::XMConvertToDegrees( entityTransformComponent.WorldRotation.z ) };

            // Snapping
            bool isSnapping = input::Input::IsKeyPressed( input::key::CtrlLeft );
            float snapValue = 0.5f;
            if ( m_GizmoType == GizmoType::Rotate )
                snapValue = 45.f;
            float snapValues[3]{ snapValue, snapValue, snapValue };

            ImGuizmo::RecomposeMatrixFromComponents(
                &entityTranslation.x, &entityRotationDeg.x, &entityScale.x, *entityTransform.m );
            ImGuizmo::Manipulate( *cameraViewMatrix.m,
                *cameraProjectionMatrix.m,
                static_cast< ImGuizmo::OPERATION >( m_GizmoType ),
                ImGuizmo::MODE::LOCAL,
                *entityTransform.m,
                nullptr,
                isSnapping ? snapValues : nullptr );

            if ( ImGuizmo::IsUsing() )
            {
                DirectX::XMFLOAT3 newEntityTranslation;
                DirectX::XMFLOAT3 newEntityRotation;
                DirectX::XMFLOAT3 newEntityScale;

                ImGuizmo::DecomposeMatrixToComponents(
                    *entityTransform.m, &newEntityTranslation.x, &newEntityRotation.x, &newEntityScale.x );
                newEntityRotation = { DirectX::XMConvertToRadians( newEntityRotation.x ),
                    DirectX::XMConvertToRadians( newEntityRotation.y ),
                    DirectX::XMConvertToRadians( newEntityRotation.z ) };

                entityTransformComponent.Translation.x += newEntityTranslation.x - entityTranslation.x;
                entityTransformComponent.Translation.y += newEntityTranslation.y - entityTranslation.y;
                entityTransformComponent.Translation.z += newEntityTranslation.z - entityTranslation.z;

                entityTransformComponent.Rotation.x += newEntityRotation.x - entityRotation.x;
                entityTransformComponent.Rotation.y += newEntityRotation.y - entityRotation.y;
                entityTransformComponent.Rotation.z += newEntityRotation.z - entityRotation.z;

                entityTransformComponent.Scale.x += newEntityScale.x - entityScale.x;
                entityTransformComponent.Scale.y += newEntityScale.y - entityScale.y;
                entityTransformComponent.Scale.z += newEntityScale.z - entityScale.z;
            }
        }

        ImGui::End();

        ImGui::PopStyleVar();

        DrawToolbar();

        ImGui::End();
    }

    void SmileEditorLayer::DrawToolbar()
    {
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0, 2 } );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0, 0 } );
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );

        const auto &colors = ImGui::GetStyle().Colors;
        const auto &buttonHoveredColor = colors[ImGuiCol_ButtonHovered];
        const auto &buttonActiveColor = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(
            ImGuiCol_ButtonHovered, ImVec4{ buttonHoveredColor.x, buttonHoveredColor.y, buttonHoveredColor.z, 0.5f } );
        ImGui::PushStyleColor(
            ImGuiCol_ButtonActive, ImVec4{ buttonActiveColor.x, buttonActiveColor.y, buttonActiveColor.z, 0.5f } );

        ImGui::Begin( "##toolbar",
            nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        const float iconSize{ ImGui::GetWindowHeight() - 4.f };
        {
            memory::Ref< graphic::Texture > pStateIcon =
                ( m_WorldState == WorldState::Edit || m_WorldState == WorldState::Simulate ) ? m_pIconPlay
                                                                                             : m_pIconStop;
            ImGui::SetCursorPosX( ( ImGui::GetContentRegionMax().x * 0.5f ) - ( iconSize * 0.5f ) );
            if ( ImGui::ImageButton( static_cast< ImTextureID >( pStateIcon->GetData() ),
                     ImVec2{ iconSize, iconSize },
                     ImVec2{ 0, 0 },
                     ImVec2{ 1, 1 },
                     0 ) )
            {
                if ( m_WorldState == WorldState::Edit || m_WorldState == WorldState::Simulate )
                    OnWorldPlay();
                else if ( m_WorldState == WorldState::Play )
                    OnWorldStop();
            }
        }
        ImGui::SameLine();
        {
            memory::Ref< graphic::Texture > pStateIcon =
                ( m_WorldState == WorldState::Edit || m_WorldState == WorldState::Play ) ? m_pIconSimulate
                                                                                         : m_pIconStop;
            if ( ImGui::ImageButton( static_cast< ImTextureID >( pStateIcon->GetData() ),
                     ImVec2{ iconSize, iconSize },
                     ImVec2{ 0, 0 },
                     ImVec2{ 1, 1 },
                     0 ) )
            {
                if ( m_WorldState == WorldState::Edit || m_WorldState == WorldState::Play )
                    OnWorldSimulate();
                else if ( m_WorldState == WorldState::Simulate )
                    OnWorldStop();
            }
        }

        ImGui::PopStyleVar( 2 );
        ImGui::PopStyleColor( 3 );
        ImGui::End();
    }

    void SmileEditorLayer::OnEvent( window::Event &e )
    {
        if ( m_IsViewportHovered )
            m_EditorCamera.OnEvent( e );

        window::EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< window::KeyPressedEvent >( SM_BIND_EVENT_FN( SmileEditorLayer::OnKeyPressed ) );
    }

    bool SmileEditorLayer::OnKeyPressed( window::KeyPressedEvent &e )
    {
        if ( e.GetRepeatCount() > 1 )
            return false;

        bool isControlPressed =
            input::Input::IsKeyPressed( input::key::CtrlLeft ) || input::Input::IsKeyPressed( input::key::CtrlRight );
        bool isShiftPressed =
            input::Input::IsKeyPressed( input::key::ShiftLeft ) || input::Input::IsKeyPressed( input::key::ShiftRight );

        switch ( e.GetKeyCode() )
        {
            case 'S':
                if ( isControlPressed )
                {
                    if ( isShiftPressed )
                        SaveWorldAs();
                    else
                        SaveWorld();
                }
                break;
            case 'O':
                if ( isControlPressed )
                    OpenProject();
                break;
            case 'N':
                if ( isControlPressed )
                    NewWorld();
                break;

            // World commands
            case 'D':
                if ( isControlPressed )
                    DuplicateEntity();
                break;

            // Gizmos
            case 'Q':
                m_GizmoType = GizmoType::None;
                break;
            case 'W':
                m_GizmoType = GizmoType::Translate;
                break;
            case 'E':
                m_GizmoType = GizmoType::Rotate;
                break;
            case 'R':
                m_GizmoType = GizmoType::Scale;
                break;
        }

        return false;
    }

    void SmileEditorLayer::NewProject()
    {
        project::ProjectManager::New();
    }

    bool SmileEditorLayer::OpenProject()
    {
        std::string filePath = window::FileDialog::OpenFile( "Smile Project (*.smproj)\0*.smproj\0" );
        if ( filePath.empty() )
            return false;

        OpenProject( filePath );
        return true;
    }

    void SmileEditorLayer::OpenProject( const std::filesystem::path &path )
    {
        if ( project::ProjectManager::Load( path ) )
        {
            // std::filesystem::current_path( path.parent_path() );

            auto startWorldPath = project::ProjectManager::GetAssetFileSystemPath(
                project::ProjectManager::GetActive()->GetConfig().StartWorld );
            OpenWorld( startWorldPath );
            m_pContentBrowserPanel = CreateScope< ContentBrowserPanel >();
        }
    }

    void SmileEditorLayer::SaveProject()
    {
        // project::Project::SaveActive();
    }

    void SmileEditorLayer::SaveWorld()
    {
        if ( !m_EditorWorldPath.empty() )
        {
            world::WorldManager::SaveActive( m_EditorWorldPath );
        }
        else
        {
            SaveWorldAs();
        }
    }

    void SmileEditorLayer::SaveWorldAs()
    {
        std::string filePath = window::FileDialog::SaveFile( "Smile World (*.smile)\0*.smile\0" );
        if ( !filePath.empty() )
        {
            world::WorldManager::SaveActive( filePath );
            m_EditorWorldPath = filePath;
        }
        else
        {
            SM_LOG_ERROR( "SmileEditorLayer::SaveWorldAs > Failed to save world. The file path was empty" );
        }
    }

    void SmileEditorLayer::OpenWorld()
    {
        std::string filePath = window::FileDialog::OpenFile( "Smile World (*.smile)\0*.smile\0" );
        if ( !filePath.empty() )
            OpenWorld( filePath );
    }

    void SmileEditorLayer::OpenWorld( const std::filesystem::path &filePath )
    {
        if ( m_WorldState != WorldState::Edit )
            OnWorldStop();

        m_pEditorWorld = world::WorldManager::Load( filePath );
        if ( m_pEditorWorld )
        {
            m_EditorWorldPath = filePath;
            m_WorldHierarchyPanel.SetContext( m_pEditorWorld.get() );
        }
    }

    void SmileEditorLayer::NewWorld()
    {
        if ( m_WorldState == WorldState::Play || m_WorldState == WorldState::Simulate )
            OnWorldStop();

        m_pEditorWorld = world::WorldManager::New();
        m_EditorWorldPath = std::filesystem::path{};

        m_WorldHierarchyPanel.SetContext( m_pEditorWorld.get() );
    }

    void SmileEditorLayer::OnWorldPlay()
    {
        if ( m_WorldState == WorldState::Simulate )
            OnWorldStop();

        m_WorldState = WorldState::Play;

        auto pActiveWorld = world::World::Copy( m_pEditorWorld );
        world::WorldManager::Open( pActiveWorld );
        pActiveWorld->ChangeState( "runtime" );

        m_WorldHierarchyPanel.SetContext( pActiveWorld.get() );
    }

    void SmileEditorLayer::OnWorldSimulate()
    {
        if ( m_WorldState == WorldState::Play )
            OnWorldStop();

        m_WorldState = WorldState::Simulate;

        auto pActiveWorld = world::World::Copy( m_pEditorWorld );
        world::WorldManager::Open( pActiveWorld );
        pActiveWorld->ChangeState( "simulate" );

        m_WorldHierarchyPanel.SetContext( pActiveWorld.get() );
    }

    void SmileEditorLayer::OnWorldStop()
    {
        if ( m_WorldState == WorldState::Play || m_WorldState == WorldState::Simulate )
            world::WorldManager::GetActive()->ChangeState( "editor" );

        m_WorldState = WorldState::Edit;
        world::WorldManager::Open( m_pEditorWorld );

        m_WorldHierarchyPanel.SetContext( m_pEditorWorld.get() );
    }

    void SmileEditorLayer::DuplicateEntity()
    {
        if ( m_WorldState != WorldState::Edit )
            return;

        world::Entity selectedEntity = m_WorldHierarchyPanel.GetSelectedEntity();
        if ( selectedEntity )
            m_pEditorWorld->DuplicateEntity( selectedEntity );
    }
}