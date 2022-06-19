#include "smile_editor_layer.h"

#include "smile_engine/scene/scene_serializer.h"
#include "smile_engine/utils/platform_utils.h"

#include <thirdparty/imgui/imgui.h>
#include <thirdparty/imguizmo/ImGuizmo.h>

namespace smile
{
    /*-----------------------------------------------------------------------------------------------------------*/
    /*---------------------------------------------- Editor Layer -----------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------------------*/

    extern const std::filesystem::path assetPath;

    SmileEditorLayer::SmileEditorLayer() : Layer( "SmileEditorLayer" )
    {
    }

    void SmileEditorLayer::onAttach()
    {
        renderer::RenderCommand::setClearColor( { DirectX::Colors::DodgerBlue.f[0],
            DirectX::Colors::DodgerBlue.f[1],
            DirectX::Colors::DodgerBlue.f[2],
            DirectX::Colors::DodgerBlue.f[3] } );

        newScene();
        editorCamera = renderer::EditorCamera{ 30.f, 1.778f, 0.1f, 2500.f };

        // Icon
        iconPlay = renderer::Texture2D::create( "resources/icons/play_button.png" );
        iconSimulate = renderer::Texture2D::create( "resources/icons/simulate_button.png" );
        iconStop = renderer::Texture2D::create( "resources/icons/stop_button.png" );
    }

    void SmileEditorLayer::onDetach()
    {
    }

    void SmileEditorLayer::onUpdate( Timestep deltaTime )
    {
        const auto &render_settings = renderer::Renderer::getSettings();
        if ( ( !math::compareFloats( viewportSize.x, static_cast< float >( render_settings.width ) ) ||
                 !math::compareFloats( viewportSize.y, static_cast< float >( render_settings.height ) ) ) &&
             ( viewportSize.x > 0 ) && ( viewportSize.y > 0 ) )
        {
            renderer::Renderer::resizeFramebuffer(
                static_cast< Uint32 >( viewportSize.x ), static_cast< Uint32 >( viewportSize.y ) );
            editorCamera.setViewportSize( viewportSize.x, viewportSize.y );
            activeScene->onViewportResize(
                static_cast< Uint32 >( viewportSize.x ), static_cast< Uint32 >( viewportSize.y ) );
        }

        renderer::RenderCommand::clear();

        switch ( sceneState )
        {
            case SceneState::Edit:
            {
                if ( viewportFocused )
                    editorCamera.onUpdate( deltaTime );

                activeScene->onUpdateEditor( deltaTime, editorCamera );
                break;
            }
            case SceneState::Simulate:
            {
                if ( viewportFocused )
                    editorCamera.onUpdate( deltaTime );

                activeScene->onUpdateSimulation( deltaTime, editorCamera );
                break;
            }
            case SceneState::Play:
            {
                activeScene->onUpdateRuntime( deltaTime );
                break;
            }
        }
    }

    void SmileEditorLayer::onImGuiRender()
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

        static bool dock_space_open = true;
        ImGui::Begin( "DockSpace Demo", &dock_space_open, window_flags );
        if ( !opt_padding )
            ImGui::PopStyleVar();

        if ( opt_fullscreen )
            ImGui::PopStyleVar( 2 );

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float standard_window_min_size = style.WindowMinSize.x;
        style.WindowMinSize.x = 350.f;

        if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
        {
            ImGuiID dockspace_id = ImGui::GetID( "MyDockSpace" );
            ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );
        }

        style.WindowMinSize.x = standard_window_min_size;

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "New", "Ctrl+N" ) )
                    newScene();

                if ( ImGui::MenuItem( "Open...", "Ctrl+O" ) )
                    openScene();

                if ( ImGui::MenuItem( "Save As...", "Ctrl+Shift+S" ) )
                    saveSceneAs();

                if ( ImGui::MenuItem( "Exit" ) )
                    smile::Application::getInstance().shutDown();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        sceneHierarchyPanel.onImGuiRender();
        contentBrowserPanel.onImGuiRender();

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{} );
        ImGui::Begin( "Viewport" );
        viewportFocused = ImGui::IsWindowFocused();
        viewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        viewportSize = { viewport_panel_size.x, viewport_panel_size.y };

        ImGui::Image( renderer::Renderer::getFinalColor(), ImVec2{ viewportSize.x, viewportSize.y } );

        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( payload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                openScene( std::filesystem::path{ assetPath } / path );
            }

            ImGui::EndDragDropTarget();
        }

        // Gizmos
        scene::Entity selected_entity = sceneHierarchyPanel.getSelectedEntity();
        if ( selected_entity && ( gizmoType != GizmoType::None ) && ( sceneState == SceneState::Edit ) )
        {
            ImGuizmo::SetOrthographic( false );
            ImGuizmo::SetDrawlist();
            float window_width = static_cast< float >( ImGui::GetWindowWidth() );
            float window_height = static_cast< float >( ImGui::GetWindowHeight() );
            ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height );

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
            const DirectX::XMFLOAT4X4 &camera_projection_matrix = editorCamera.getProjectionMatrix();
            const DirectX::XMFLOAT4X4 &camera_view_matrix = editorCamera.getViewMatrix();

            // Entity
            auto &entity_transform_component = selected_entity.getComponent< scene::TransformComponent >();
            auto entity_transform = entity_transform_component.getTransform();
            auto entity_rotation = DirectX::XMFLOAT3{ DirectX::XMConvertToDegrees( entity_transform_component.rotation.x ),
                DirectX::XMConvertToDegrees( entity_transform_component.rotation.y ),
                DirectX::XMConvertToDegrees( entity_transform_component.rotation.z ) };

            // Snapping
            bool snapping = input::Input::isKeyPressed( input::key::CtrlLeft );
            float snap_value = 0.5f;
            if ( gizmoType == GizmoType::Rotate )
                snap_value = 45.f;
            float snap_values[3]{ snap_value, snap_value, snap_value };

            ImGuizmo::RecomposeMatrixFromComponents( &entity_transform_component.translation.x,
                &entity_rotation.x,
                &entity_transform_component.scale.x,
                *entity_transform.m );
            ImGuizmo::Manipulate( *camera_view_matrix.m,
                *camera_projection_matrix.m,
                static_cast< ImGuizmo::OPERATION >( gizmoType ),
                ImGuizmo::MODE::LOCAL,
                *entity_transform.m,
                nullptr,
                snapping ? snap_values : nullptr );

            if ( ImGuizmo::IsUsing() )
            {
                ImGuizmo::DecomposeMatrixToComponents( *entity_transform.m,
                    &entity_transform_component.translation.x,
                    &entity_rotation.x,
                    &entity_transform_component.scale.x );
                entity_transform_component.rotation = { DirectX::XMConvertToRadians( entity_rotation.x ),
                    DirectX::XMConvertToRadians( entity_rotation.y ),
                    DirectX::XMConvertToRadians( entity_rotation.z ) };
            }
        }

        ImGui::End();

        ImGui::PopStyleVar();

        drawToolbar();

        ImGui::End();
    }

    void SmileEditorLayer::drawToolbar()
    {
        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0, 2 } );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0, 0 } );
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 } );

        const auto &colors = ImGui::GetStyle().Colors;
        const auto &button_hovered_color = colors[ImGuiCol_ButtonHovered];
        const auto &button_active_color = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(
            ImGuiCol_ButtonHovered, ImVec4{ button_hovered_color.x, button_hovered_color.y, button_hovered_color.z, 0.5f } );
        ImGui::PushStyleColor(
            ImGuiCol_ButtonActive, ImVec4{ button_active_color.x, button_active_color.y, button_active_color.z, 0.5f } );

        ImGui::Begin( "##toolbar",
            nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

        const float icon_size{ ImGui::GetWindowHeight() - 4.f };
        {
            Ref< renderer::Texture2D > state_icon =
                ( sceneState == SceneState::Edit || sceneState == SceneState::Simulate ) ? iconPlay : iconStop;
            ImGui::SetCursorPosX( ( ImGui::GetContentRegionMax().x * 0.5f ) - ( icon_size * 0.5f ) );
            if ( ImGui::ImageButton( static_cast< ImTextureID >( state_icon->getData() ),
                     ImVec2{ icon_size, icon_size },
                     ImVec2{ 0, 0 },
                     ImVec2{ 1, 1 },
                     0 ) )
            {
                if ( sceneState == SceneState::Edit || sceneState == SceneState::Simulate )
                    onScenePlay();
                else if ( sceneState == SceneState::Play )
                    onSceneStop();
            }
        }
        ImGui::SameLine();
        {
            Ref< renderer::Texture2D > state_icon =
                ( sceneState == SceneState::Edit || sceneState == SceneState::Play ) ? iconSimulate : iconStop;
            if ( ImGui::ImageButton( static_cast< ImTextureID >( state_icon->getData() ),
                     ImVec2{ icon_size, icon_size },
                     ImVec2{ 0, 0 },
                     ImVec2{ 1, 1 },
                     0 ) )
            {
                if ( sceneState == SceneState::Edit || sceneState == SceneState::Play )
                    onSceneSimulate();
                else if ( sceneState == SceneState::Simulate )
                    onSceneStop();
            }
        }

        ImGui::PopStyleVar( 2 );
        ImGui::PopStyleColor( 3 );
        ImGui::End();
    }

    void SmileEditorLayer::onEvent( Event &e )
    {
        if ( viewportHovered )
            editorCamera.onEvent( e );

        EventDispatcher dispatcher{ e };
        dispatcher.dispatch< KeyPressedEvent >( SM_BIND_EVENT_FN( SmileEditorLayer::onKeyPressed ) );
    }

    bool SmileEditorLayer::onKeyPressed( KeyPressedEvent &e )
    {
        if ( e.getRepeatCount() > 1 )
            return false;

        bool control_pressed = input::Input::isKeyPressed( input::key::CtrlLeft ) || input::Input::isKeyPressed( input::key::CtrlRight );
        bool shift_pressed = input::Input::isKeyPressed( input::key::ShiftLeft ) || input::Input::isKeyPressed( input::key::ShiftRight );

        switch ( e.getKeyCode() )
        {
            case 'S':
                if ( control_pressed )
                {
                    if ( shift_pressed )
                        saveSceneAs();
                    else
                        saveScene();
                }
                break;
            case 'O':
                if ( control_pressed )
                    openScene();
                break;
            case 'N':
                if ( control_pressed )
                    newScene();
                break;

            // Scene commands
            case 'D':
                if ( control_pressed )
                    duplicateEntity();
                break;

            // Gizmos
            case 'Q':
                gizmoType = GizmoType::None;
                break;
            case 'W':
                gizmoType = GizmoType::Translate;
                break;
            case 'E':
                gizmoType = GizmoType::Rotate;
                break;
            case 'R':
                gizmoType = GizmoType::Scale;
                break;
        }

        return false;
    }

    void SmileEditorLayer::saveScene()
    {
        if ( !editorScenePath.empty() )
        {
            serializeScene( activeScene, editorScenePath );
        }
        else
            saveSceneAs();
    }

    void SmileEditorLayer::saveSceneAs()
    {
        std::string file_path = utils::saveFile( "Smile Scene (*.smile)\0*.smile\0" );
        if ( !file_path.empty() )
        {
            serializeScene( activeScene, file_path );
            editorScenePath = file_path;
        }
        else
            SM_LOG_ERROR( "SmileEditorLayer::saveSceneAs > Failed to save scene. The file path was empty" );
    }

    void SmileEditorLayer::serializeScene( const Ref< scene::Scene > &scene, const std::filesystem::path &file_path )
    {
        scene::SceneSerializer scene_serializer{ scene };
        scene_serializer.serialize( file_path.string() );
    }

    void SmileEditorLayer::openScene()
    {
        std::string file_path = utils::openFile( "Smile Scene (*.smile)\0*.smile\0" );
        openScene( file_path );
    }

    void SmileEditorLayer::openScene( const std::filesystem::path &file_path )
    {
        if ( sceneState != SceneState::Edit )
            onSceneStop();

        if ( file_path.empty() )
        {
            SM_LOG_WARNING( "SmileEditorLayer::openScene > Failed to load scene: the path was empty" );
            return;
        }

        if ( file_path.extension().string() != ".smile" )
        {
            SM_LOG_WARNING( "SmileEditorLayer::openScene > Failed to load scene: wrong file extention" );
            return;
        }

        Ref< scene::Scene > new_scene = createRef< scene::Scene >();
        scene::SceneSerializer scene_serializer{ new_scene };
        if ( scene_serializer.deserialize( file_path.string() ) )
        {
            editorScene = new_scene;

            editorScene->onViewportResize(
                static_cast< Uint32 >( viewportSize.x ), static_cast< Uint32 >( viewportSize.y ) );
            sceneHierarchyPanel.setContext( editorScene );
            editorCamera.setViewportSize( viewportSize.x, viewportSize.y );

            activeScene = editorScene;
            editorScenePath = file_path;
        }
    }

    void SmileEditorLayer::newScene()
    {
        if ( sceneState == SceneState::Play )
            onSceneStop();

        activeScene = createRef< scene::Scene >();
        editorScene = activeScene;
        activeScene->onViewportResize(
            static_cast< Uint32 >( viewportSize.x ), static_cast< Uint32 >( viewportSize.y ) );
        editorCamera.setViewportSize( viewportSize.x, viewportSize.y );
        sceneHierarchyPanel.setContext( activeScene );

        editorScenePath = std::filesystem::path{};
    }

    void SmileEditorLayer::onScenePlay()
    {
        if ( sceneState == SceneState::Simulate )
            onSceneStop();

        sceneState = SceneState::Play;
        activeScene = scene::Scene::copy( editorScene );
        activeScene->onRuntimeStart();
        sceneHierarchyPanel.setContext( activeScene );
    }

    void SmileEditorLayer::onSceneSimulate()
    {
        if ( sceneState == SceneState::Play )
            onSceneStop();

        sceneState = SceneState::Simulate;
        activeScene = scene::Scene::copy( editorScene );
        activeScene->onSimulationStart();
        sceneHierarchyPanel.setContext( activeScene );
    }

    void SmileEditorLayer::onSceneStop()
    {
        if ( sceneState == SceneState::Play )
            activeScene->onRuntimeStop();
        else if ( sceneState == SceneState::Simulate )
            activeScene->onSimulationStop();

        sceneState = SceneState::Edit;
        activeScene = editorScene;
        sceneHierarchyPanel.setContext( activeScene );
    }

    void SmileEditorLayer::duplicateEntity()
    {
        if ( sceneState != SceneState::Edit )
            return;

        scene::Entity selected_entity = sceneHierarchyPanel.getSelectedEntity();
        if ( selected_entity )
            editorScene->duplicateEntity( selected_entity );
    }
}