/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile_editor_layer.h"

#include "smile_engine/scene/scene_serializer.h"
#include "smile_engine/utils/platform_utils.h"

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

namespace smile
{
    /*-----------------------------------------------------------------------------------------------------------*/
    /*---------------------------------------------- Editor Layer -----------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------------*/

    SmileEditorLayer::SmileEditorLayer() : Layer( "SmileEditorLayer" )
    {
    }

    void SmileEditorLayer::OnAttach()
    {
        graphic::RenderCommand::SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
            DirectX::Colors::DodgerBlue.f[1],
            DirectX::Colors::DodgerBlue.f[2],
            DirectX::Colors::DodgerBlue.f[3] } );

        NewScene();
        m_EditorCamera = graphic::EditorCamera{ 30.f, 1.778f, 0.1f, 2500.f };

        // Icon
        auto pDevice = graphic::RenderEngine::GetDevice();
        m_pIconPlay = pDevice->CreateTexture2D( "resources/icons/play_button.png" );
        m_pIconSimulate = pDevice->CreateTexture2D( "resources/icons/simulate_button.png" );
        m_pIconStop = pDevice->CreateTexture2D( "resources/icons/stop_button.png" );

        auto commandLineArgs = Application::GetInstance().GetDescriptor().CommandLineArgs;
        if (commandLineArgs.Count > 1)
        {
            auto projectFilePath = commandLineArgs[1];
            OpenProject( projectFilePath );
        }
        else
        {
            // TODO: prompt the user to select a directory
            // NewProject();

            if ( !OpenProject() )
                Application::GetInstance().ShutDown();
        }
    }

    void SmileEditorLayer::OnDetach()
    {
    }

    void SmileEditorLayer::OnUpdate( Timestep deltaTime )
    {
        const auto &renderSettings = graphic::RenderEngine::GetSettings();
        if ( ( !math::AreEqual( m_ViewportSize.x, static_cast< float >( renderSettings.Width ) ) ||
                 !math::AreEqual( m_ViewportSize.y, static_cast< float >( renderSettings.Height ) ) ) &&
             ( m_ViewportSize.x > 0 ) && ( m_ViewportSize.y > 0 ) )
        {
            graphic::RenderEngine::ResizeFramebuffer(
                static_cast< Uint32 >( m_ViewportSize.x ), static_cast< Uint32 >( m_ViewportSize.y ) );
            m_EditorCamera.SetViewportSize( m_ViewportSize.x, m_ViewportSize.y );
            m_pActiveScene->OnViewportResize(
                static_cast< Uint32 >( m_ViewportSize.x ), static_cast< Uint32 >( m_ViewportSize.y ) );
        }

        graphic::RenderCommand::Clear();

        switch ( m_SceneState )
        {
            case SceneState::Edit:
            {
                if ( m_IsViewportFocused )
                    m_EditorCamera.OnUpdate( deltaTime );

                m_pActiveScene->OnUpdateEditor( deltaTime, m_EditorCamera );
                break;
            }
            case SceneState::Simulate:
            {
                if ( m_IsViewportFocused )
                    m_EditorCamera.OnUpdate( deltaTime );

                m_pActiveScene->OnUpdateSimulation( deltaTime, m_EditorCamera );
                break;
            }
            case SceneState::Play:
            {
                m_pActiveScene->OnUpdateRuntime( deltaTime );
                break;
            }
        }
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
            ImGuiID dockspace_id = ImGui::GetID( "MyDockSpace" );
            ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspaceFlags );
        }

        style.WindowMinSize.x = standardWindowMinSize;

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "Open Project...", "Ctrl+O" ) )
                    OpenProject();

                ImGui::Separator();

                if ( ImGui::MenuItem( "New Scene", "Ctrl+N" ) )
                    NewScene();

                if ( ImGui::MenuItem( "Save Scene", "Ctrl+S" ) )
                    SaveScene();

                if ( ImGui::MenuItem( "Save Scene As...", "Ctrl+Shift+S" ) )
                    SaveSceneAs();

                ImGui::Separator();

                if ( ImGui::MenuItem( "Exit" ) )
                    smile::Application::GetInstance().ShutDown();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();
        m_pContentBrowserPanel->OnImGuiRender();

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{} );
        ImGui::Begin( "Viewport" );
        m_IsViewportFocused = ImGui::IsWindowFocused();
        m_IsViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image( graphic::RenderEngine::GetFinalColor(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y } );

        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( pPayload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                OpenScene( path );
            }

            ImGui::EndDragDropTarget();
        }

        // Gizmos
        scene::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if ( selectedEntity && ( m_GizmoType != GizmoType::None ) && ( m_SceneState == SceneState::Edit ) )
        {
            ImGuizmo::SetOrthographic( false );
            ImGuizmo::SetDrawlist();
            float windowWidth = static_cast< float >( ImGui::GetWindowWidth() );
            float windowHeight = static_cast< float >( ImGui::GetWindowHeight() );
            ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight );

            //// Runtime camera
            // auto cameraEntity = m_pActiveScene->GetPrimaryCameraEntity();
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
            auto &entityTransformComponent = selectedEntity.GetComponent< scene::TransformComponent >();
            auto entityTransform = entityTransformComponent.GetTransform();
            auto entityRotation = DirectX::XMFLOAT3{ DirectX::XMConvertToDegrees( entityTransformComponent.Rotation.x ),
                DirectX::XMConvertToDegrees( entityTransformComponent.Rotation.y ),
                DirectX::XMConvertToDegrees( entityTransformComponent.Rotation.z ) };

            // Snapping
            bool isSnapping = input::Input::IsKeyPressed( input::key::CtrlLeft );
            float snapValue = 0.5f;
            if ( m_GizmoType == GizmoType::Rotate )
                snapValue = 45.f;
            float snapValues[3]{ snapValue, snapValue, snapValue };

            ImGuizmo::RecomposeMatrixFromComponents( &entityTransformComponent.Translation.x,
                &entityRotation.x,
                &entityTransformComponent.Scale.x,
                *entityTransform.m );
            ImGuizmo::Manipulate( *cameraViewMatrix.m,
                *cameraProjectionMatrix.m,
                static_cast< ImGuizmo::OPERATION >( m_GizmoType ),
                ImGuizmo::MODE::LOCAL,
                *entityTransform.m,
                nullptr,
                isSnapping ? snapValues : nullptr );

            if ( ImGuizmo::IsUsing() )
            {
                ImGuizmo::DecomposeMatrixToComponents( *entityTransform.m,
                    &entityTransformComponent.Translation.x,
                    &entityRotation.x,
                    &entityTransformComponent.Scale.x );
                entityTransformComponent.Rotation = { DirectX::XMConvertToRadians( entityRotation.x ),
                    DirectX::XMConvertToRadians( entityRotation.y ),
                    DirectX::XMConvertToRadians( entityRotation.z ) };
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
            Ref< graphic::Texture2D > pStateIcon =
                ( m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate ) ? m_pIconPlay : m_pIconStop;
            ImGui::SetCursorPosX( ( ImGui::GetContentRegionMax().x * 0.5f ) - ( iconSize * 0.5f ) );
            if ( ImGui::ImageButton( static_cast< ImTextureID >( pStateIcon->GetData() ),
                     ImVec2{ iconSize, iconSize },
                     ImVec2{ 0, 0 },
                     ImVec2{ 1, 1 },
                     0 ) )
            {
                if ( m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate )
                    OnScenePlay();
                else if ( m_SceneState == SceneState::Play )
                    OnSceneStop();
            }
        }
        ImGui::SameLine();
        {
            Ref< graphic::Texture2D > pStateIcon =
                ( m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play ) ? m_pIconSimulate : m_pIconStop;
            if ( ImGui::ImageButton( static_cast< ImTextureID >( pStateIcon->GetData() ),
                     ImVec2{ iconSize, iconSize },
                     ImVec2{ 0, 0 },
                     ImVec2{ 1, 1 },
                     0 ) )
            {
                if ( m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play )
                    OnSceneSimulate();
                else if ( m_SceneState == SceneState::Simulate )
                    OnSceneStop();
            }
        }

        ImGui::PopStyleVar( 2 );
        ImGui::PopStyleColor( 3 );
        ImGui::End();
    }

    void SmileEditorLayer::OnEvent( Event &e )
    {
        if ( m_IsViewportHovered )
            m_EditorCamera.OnEvent( e );

        EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< KeyPressedEvent >( SM_BIND_EVENT_FN( SmileEditorLayer::OnKeyPressed ) );
    }

    bool SmileEditorLayer::OnKeyPressed( KeyPressedEvent &e )
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
                        SaveSceneAs();
                    else
                        SaveScene();
                }
                break;
            case 'O':
                if ( isControlPressed )
                    OpenProject();
                break;
            case 'N':
                if ( isControlPressed )
                    NewScene();
                break;

            // Scene commands
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
        std::string filePath = utils::OpenFile( "Smile Project (*.smproj)\0*.smproj\0" );
        if ( filePath.empty() )
            return false;

        OpenProject( filePath );
        return true;
    }

    void SmileEditorLayer::OpenProject( const std::filesystem::path &path )
    {
        if ( project::ProjectManager::Load( path ) )
        {
            //std::filesystem::current_path( path.parent_path() );

            auto startScenePath =
                project::ProjectManager::GetAssetFileSystemPath( project::ProjectManager::GetActive()->GetConfig().StartScene );
            OpenScene( startScenePath );
            m_pContentBrowserPanel = CreateScope< ContentBrowserPanel >();
        }
    }

    void SmileEditorLayer::SaveProject()
    {
        //project::Project::SaveActive();
    }

    void SmileEditorLayer::SaveScene()
    {
        if ( !m_EditorScenePath.empty() )
        {
            SerializeScene( m_pActiveScene, m_EditorScenePath );
        }
        else
            SaveSceneAs();
    }

    void SmileEditorLayer::SaveSceneAs()
    {
        std::string filePath = utils::SaveFile( "Smile Scene (*.smile)\0*.smile\0" );
        if ( !filePath.empty() )
        {
            SerializeScene( m_pActiveScene, filePath );
            m_EditorScenePath = filePath;
        }
        else
            SM_LOG_ERROR( "SmileEditorLayer::SaveSceneAs > Failed to save scene. The file path was empty" );
    }

    void SmileEditorLayer::SerializeScene( const Ref< scene::Scene > &pScene, const std::filesystem::path &filePath )
    {
        scene::SceneSerializer sceneSerializer{ pScene };
        sceneSerializer.Serialize( filePath.string() );
    }

    void SmileEditorLayer::OpenScene()
    {
        std::string filePath = utils::OpenFile( "Smile Scene (*.smile)\0*.smile\0" );
        if ( !filePath.empty() )
            OpenScene( filePath );
    }

    void SmileEditorLayer::OpenScene( const std::filesystem::path &filePath )
    {
        if ( m_SceneState != SceneState::Edit )
            OnSceneStop();

        if ( filePath.empty() )
        {
            SM_LOG_WARNING( "SmileEditorLayer::OpenScene > Failed to load scene: the path was empty" );
            return;
        }

        if ( filePath.extension().string() != ".smile" )
        {
            SM_LOG_WARNING( "SmileEditorLayer::OpenScene > Failed to load scene: wrong file extention" );
            return;
        }

        Ref< scene::Scene > pNewScene = CreateRef< scene::Scene >();
        scene::SceneSerializer sceneSerializer{ pNewScene };
        if ( sceneSerializer.Deserialize( filePath.string() ) )
        {
            m_pEditorScene = pNewScene;

            m_pEditorScene->OnViewportResize(
                static_cast< Uint32 >( m_ViewportSize.x ), static_cast< Uint32 >( m_ViewportSize.y ) );
            m_pEditorScene->OnOpen();
            m_SceneHierarchyPanel.SetContext( m_pEditorScene );
            m_EditorCamera.SetViewportSize( m_ViewportSize.x, m_ViewportSize.y );

            m_pActiveScene = m_pEditorScene;
            m_EditorScenePath = filePath;
        }
    }

    void SmileEditorLayer::NewScene()
    {
        if ( m_SceneState == SceneState::Play )
            OnSceneStop();

        m_pActiveScene = CreateRef< scene::Scene >();
        m_pEditorScene = m_pActiveScene;
        m_pActiveScene->OnViewportResize(
            static_cast< Uint32 >( m_ViewportSize.x ), static_cast< Uint32 >( m_ViewportSize.y ) );
        m_EditorCamera.SetViewportSize( m_ViewportSize.x, m_ViewportSize.y );
        m_pActiveScene->OnOpen();
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );

        m_EditorScenePath = std::filesystem::path{};
    }

    void SmileEditorLayer::OnScenePlay()
    {
        if ( m_SceneState == SceneState::Simulate )
            OnSceneStop();

        m_SceneState = SceneState::Play;
        m_pActiveScene = scene::Scene::Copy( m_pEditorScene );
        m_pActiveScene->OnOpen();
        m_pActiveScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );
    }

    void SmileEditorLayer::OnSceneSimulate()
    {
        if ( m_SceneState == SceneState::Play )
            OnSceneStop();

        m_SceneState = SceneState::Simulate;
        m_pActiveScene = scene::Scene::Copy( m_pEditorScene );
        m_pActiveScene->OnOpen();
        m_pActiveScene->OnSimulationStart();
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );
    }

    void SmileEditorLayer::OnSceneStop()
    {
        if ( m_SceneState == SceneState::Play )
            m_pActiveScene->OnRuntimeStop();
        else if ( m_SceneState == SceneState::Simulate )
            m_pActiveScene->OnSimulationStop();

        m_SceneState = SceneState::Edit;
        m_pActiveScene = m_pEditorScene;
        m_pActiveScene->OnOpen();
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );
    }

    void SmileEditorLayer::DuplicateEntity()
    {
        if ( m_SceneState != SceneState::Edit )
            return;

        scene::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if ( selectedEntity )
            m_pEditorScene->DuplicateEntity( selectedEntity );
    }
}