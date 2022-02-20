#include "SmileEditorLayer.h"

#include "SmileEngine/Scene/SceneSerializer.h"
#include "SmileEngine/Utils/PlatformUtils.h"

#include <Libs/ImGui/imgui.h>
#include <Libs/ImGuizmo/ImGuizmo.h>

namespace smile
{
    /*-----------------------------------------------------------------------------------------------------------*/
    /*---------------------------------------------- Editor Layer -----------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------------*/

    extern const std::filesystem::path g_ResourcePath;

    SmileEditorLayer::SmileEditorLayer() : Layer( "SmileEditorLayer" )
    {
    }

    void SmileEditorLayer::OnAttach()
    {
        smile::RenderCommand::SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
            DirectX::Colors::DodgerBlue.f[1],
            DirectX::Colors::DodgerBlue.f[2],
            DirectX::Colors::DodgerBlue.f[3] } );

        NewScene();
        m_EditorCamera = EditorCamera{ 30.f, 1.778f, 0.1f, 2500.f };

        // Icon
        m_pIconPlay = Texture2D::Create( "EditorResources/Icons/PlayButton.png" );
        m_pIconStop = Texture2D::Create( "EditorResources/Icons/StopButton.png" );
    }

    void SmileEditorLayer::OnDetach()
    {
    }

    void SmileEditorLayer::OnUpdate( Timestep deltaTime )
    {
        const auto &renderSettings = Renderer::GetSettings();
        if ( ( !Math::CompareFloats( m_ViewportSize.x, static_cast< float >( renderSettings.m_Width ) ) ||
                 !Math::CompareFloats( m_ViewportSize.y, static_cast< float >( renderSettings.m_Height ) ) ) &&
             ( m_ViewportSize.x > 0 ) && ( m_ViewportSize.y > 0 ) )
        {
            Renderer::ResizeFramebuffer(
                static_cast< uint32_t >( m_ViewportSize.x ), static_cast< uint32_t >( m_ViewportSize.y ) );
            m_EditorCamera.SetViewportSize( m_ViewportSize.x, m_ViewportSize.y );
            m_pActiveScene->OnViewportResize(
                static_cast< uint32_t >( m_ViewportSize.x ), static_cast< uint32_t >( m_ViewportSize.y ) );
        }

        smile::RenderCommand::Clear();

        switch ( m_SceneState )
        {
            case SceneState::Edit:
            {
                if ( m_bViewportFocused )
                    m_EditorCamera.OnUpdate( deltaTime );

                m_pActiveScene->OnUpdateEditor( deltaTime, m_EditorCamera );
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
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if ( opt_fullscreen )
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos( viewport->GetWorkPos() );
            ImGui::SetNextWindowSize( viewport->GetWorkSize() );
            ImGui::SetNextWindowViewport( viewport->ID );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if ( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode )
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if ( !opt_padding )
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );

        static bool bDockSpaceOpen = true;
        ImGui::Begin( "DockSpace Demo", &bDockSpaceOpen, window_flags );
        if ( !opt_padding )
            ImGui::PopStyleVar();

        if ( opt_fullscreen )
            ImGui::PopStyleVar( 2 );

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float standardWindowMinSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 350.f;

        if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
        {
            ImGuiID dockspace_id = ImGui::GetID( "MyDockSpace" );
            ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );
        }

        style.WindowMinSize.x = standardWindowMinSize;

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "New", "Ctrl+N" ) )
                    NewScene();

                if ( ImGui::MenuItem( "Open...", "Ctrl+O" ) )
                    OpenScene();

                if ( ImGui::MenuItem( "Save As...", "Ctrl+Shift+S" ) )
                    SaveSceneAs();

                if ( ImGui::MenuItem( "Exit" ) )
                    smile::Application::GetInstance().ShutDown();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{} );
        ImGui::Begin( "Viewport" );
        m_bViewportFocused = ImGui::IsWindowFocused();
        m_bViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image( Renderer::GetFinalColor(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y } );

        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( pPayload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                OpenScene( std::filesystem::path{ g_ResourcePath } / path );
            }

            ImGui::EndDragDropTarget();
        }

        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
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
            auto &entityTransformComponent = selectedEntity.GetComponent< TransformComponent >();
            auto entityTransform = entityTransformComponent.GetTransform();
            auto entityRotation = DirectX::XMFLOAT3{ DirectX::XMConvertToDegrees( entityTransformComponent.m_Rotation.x ),
                DirectX::XMConvertToDegrees( entityTransformComponent.m_Rotation.y ),
                DirectX::XMConvertToDegrees( entityTransformComponent.m_Rotation.z ) };

            // Snapping
            bool bSnapping = Input::IsKeyPressed( Key::CtrlLeft );
            float snapValue = 0.5f;
            if ( m_GizmoType == GizmoType::Rotate )
                snapValue = 45.f;
            float snapValues[3]{ snapValue, snapValue, snapValue };

            ImGuizmo::RecomposeMatrixFromComponents( &entityTransformComponent.m_Translation.x,
                &entityRotation.x,
                &entityTransformComponent.m_Scale.x,
                *entityTransform.m );
            ImGuizmo::Manipulate( *cameraViewMatrix.m,
                *cameraProjectionMatrix.m,
                static_cast< ImGuizmo::OPERATION >( m_GizmoType ),
                ImGuizmo::MODE::LOCAL,
                *entityTransform.m,
                nullptr,
                bSnapping ? snapValues : nullptr );

            if ( ImGuizmo::IsUsing() )
            {
                ImGuizmo::DecomposeMatrixToComponents( *entityTransform.m,
                    &entityTransformComponent.m_Translation.x,
                    &entityRotation.x,
                    &entityTransformComponent.m_Scale.x );
                entityTransformComponent.m_Rotation = { DirectX::XMConvertToRadians( entityRotation.x ),
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

        Ref< Texture2D > pStateIcon = ( m_SceneState == SceneState::Edit ) ? m_pIconPlay : m_pIconStop;
        float iconSize{ ImGui::GetWindowHeight() - 4.f };
        ImGui::SetCursorPosX( ( ImGui::GetContentRegionMax().x * 0.5f ) - ( iconSize * 0.5f ) );
        if ( ImGui::ImageButton( static_cast< ImTextureID >( pStateIcon->GetData() ),
                 ImVec2{ iconSize, iconSize },
                 ImVec2{ 0, 0 },
                 ImVec2{ 1, 1 },
                 0 ) )
        {
            if ( m_SceneState == SceneState::Edit )
                OnScenePlay();
            else if ( m_SceneState == SceneState::Play )
                OnSceneStop();
        }

        ImGui::PopStyleVar( 2 );
        ImGui::PopStyleColor( 3 );
        ImGui::End();
    }

    void SmileEditorLayer::OnEvent( Event &e )
    {
        if ( m_bViewportHovered )
            m_EditorCamera.OnEvent( e );

        EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< KeyPressedEvent >( SM_BIND_EVENT_FN( SmileEditorLayer::OnKeyPressed ) );
    }

    bool SmileEditorLayer::OnKeyPressed( KeyPressedEvent &e )
    {
        if ( e.GetRepeatCount() > 1 )
            return false;

        bool bControlPressed = Input::IsKeyPressed( Key::CtrlLeft ) || Input::IsKeyPressed( Key::CtrlRight );
        bool bShiftPressed = Input::IsKeyPressed( Key::ShiftLeft ) || Input::IsKeyPressed( Key::ShiftRight );

        switch ( e.GetKeyCode() )
        {
            case 'S':
                if ( bControlPressed )
                {
                    if ( bShiftPressed )
                        SaveSceneAs();
                    else
                        SaveScene();
                }
                break;
            case 'O':
                if ( bControlPressed )
                    OpenScene();
                break;
            case 'N':
                if ( bControlPressed )
                    NewScene();
                break;

            // Scene commands
            case 'D':
                if ( bControlPressed )
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

    void SmileEditorLayer::SerializeScene( const Ref< Scene > &pScene, const std::filesystem::path &filePath )
    {
        SceneSerializer sceneSerializer{ pScene };
        sceneSerializer.Serialize( filePath.string() );
    }

    void SmileEditorLayer::OpenScene()
    {
        std::string filePath = utils::OpenFile( "Smile Scene (*.smile)\0*.smile\0" );
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

        Ref< Scene > pNewScene = CreateRef< Scene >();
        SceneSerializer sceneSerializer{ pNewScene };
        if ( sceneSerializer.Deserialize( filePath.string() ) )
        {
            m_pEditorScene = pNewScene;

            m_pEditorScene->OnViewportResize(
                static_cast< uint32_t >( m_ViewportSize.x ), static_cast< uint32_t >( m_ViewportSize.y ) );
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

        m_pActiveScene = CreateRef< Scene >();
        m_pEditorScene = m_pActiveScene;
        m_pActiveScene->OnViewportResize(
            static_cast< uint32_t >( m_ViewportSize.x ), static_cast< uint32_t >( m_ViewportSize.y ) );
        m_EditorCamera.SetViewportSize( m_ViewportSize.x, m_ViewportSize.y );
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );

        m_EditorScenePath = std::filesystem::path{};
    }

    void SmileEditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;
        m_pActiveScene = Scene::Copy( m_pEditorScene );
        m_pActiveScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );
    }

    void SmileEditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;
        m_pActiveScene->OnRuntimeStop();
        m_pActiveScene = m_pEditorScene;
        m_SceneHierarchyPanel.SetContext( m_pActiveScene );
    }

    void SmileEditorLayer::DuplicateEntity()
    {
        if ( m_SceneState != SceneState::Edit )
            return;

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if ( selectedEntity )
            m_pEditorScene->DuplicateEntity( selectedEntity );
    }
}