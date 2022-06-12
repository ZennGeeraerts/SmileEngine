#include "scene_hierarchy_panel.h"

#include <thirdparty/imgui/imgui.h>
#include <thirdparty/imgui/imgui_internal.h>

#include "smile_engine/scene/components.h"
#include "content_browser_panel.h"

namespace smile
{
    extern const std::filesystem::path assetPath;
}

namespace smile::scene
{
    SceneHierarchyPanel::SceneHierarchyPanel( const Ref< Scene > &scene )
    {
        setContext( scene );
    }

    void SceneHierarchyPanel::setContext( const Ref< Scene > &scene )
    {
        context = scene;
        selectedEntity = {};
    }

    void SceneHierarchyPanel::onImGuiRender()
    {
        ImGui::Begin( "Scene Hierarchy" );

        if ( context )
        {
            context->registry.each(
                [&]( auto entity_id )
                {
                    Entity entity{ entity_id, context.get() };
                    drawEntityNode( entity );
                } );

            // Deselect entities
            if ( ImGui::IsMouseDown( 0 ) && ImGui::IsWindowHovered() )
                selectedEntity = {};

            // Right click on blank space
            if ( ImGui::BeginPopupContextWindow( 0, 1, false ) )
            {
                if ( ImGui::MenuItem( "Create Empty Entity" ) )
                    context->createEntity();

                ImGui::EndPopup();
            }
        }

        ImGui::End();

        // Inspector Panel
        ImGui::Begin( "Inspector" );

        // Add Components
        if ( selectedEntity )
        {
            drawComponents( selectedEntity );
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::drawEntityNode( Entity entity )
    {
        auto &tag = entity.getComponent< TagComponent >().tag;

        const ImGuiTreeNodeFlags flags = ( ( selectedEntity == entity ) ? ImGuiTreeNodeFlags_Selected : 0 ) |
                                         ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool node_expanded = ImGui::TreeNodeEx( ( void * )( Uint64 )( Uint32 )entity, flags, tag.c_str() );
        if ( ImGui::IsItemClicked() )
        {
            selectedEntity = entity;
        }

        bool entity_deleted = false;
        if ( ImGui::BeginPopupContextItem() )
        {
            if ( ImGui::MenuItem( "Delete Entity" ) )
                entity_deleted = true;

            ImGui::EndPopup();
        }

        if ( node_expanded )
        {
            ImGui::TreePop();
        }

        if ( entity_deleted )
        {
            context->destroyEntity( entity );
            if ( selectedEntity == entity )
                selectedEntity = {};
        }
    }

    void SceneHierarchyPanel::drawVector3Control( const std::string &label,
        DirectX::XMFLOAT3 &value,
        float reset_value,
        float column_width )
    {
        ImGuiIO &io = ImGui::GetIO();
        auto bold_font = io.Fonts->Fonts[1];

        ImGui::PushID( label.c_str() );

        ImGui::Columns( 2 );

        ImGui::SetColumnWidth( 0, column_width );
        ImGui::Text( label.c_str() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );

        float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 button_size{ line_height + 3.0f, line_height };

        // X
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f } );
        ImGui::PushFont( bold_font );

        if ( ImGui::Button( "X", button_size ) )
            value.x = reset_value;

        ImGui::PopFont();
        ImGui::PopStyleColor( 3 );

        ImGui::SameLine();
        ImGui::DragFloat( "##X", &value.x, 0.03f );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Y
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f } );
        ImGui::PushFont( bold_font );

        if ( ImGui::Button( "Y", button_size ) )
            value.y = reset_value;

        ImGui::PopFont();
        ImGui::PopStyleColor( 3 );

        ImGui::SameLine();
        ImGui::DragFloat( "##Y", &value.y, 0.03f );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Z
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f } );
        ImGui::PushFont( bold_font );

        if ( ImGui::Button( "Z", button_size ) )
            value.z = reset_value;

        ImGui::PopFont();
        ImGui::PopStyleColor( 3 );

        ImGui::SameLine();
        ImGui::DragFloat( "##Z", &value.z, 0.03f );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns( 1 );

        ImGui::PopID();
    }

    void SceneHierarchyPanel::drawComponents( Entity entity )
    {
        if ( entity.hasComponent< TagComponent >() )
        {
            auto &tag = entity.getComponent< TagComponent >().tag;

            char tag_buffer[256];
            memset( tag_buffer, 0, sizeof( tag_buffer ) );
            strcpy_s( tag_buffer, sizeof( tag_buffer ), tag.c_str() );

            if ( ImGui::InputText( "##Tag", tag_buffer, sizeof( tag_buffer ) ) )
            {
                tag = std::string{ tag_buffer };
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth( -1 );
        if ( ImGui::Button( "Add Component" ) )
            ImGui::OpenPopup( "AddComponent" );

        if ( ImGui::BeginPopup( "AddComponent" ) )
        {
            if ( ImGui::MenuItem( "Camera" ) )
            {
                selectedEntity.addComponent< CameraComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Static Mesh" ) )
            {
                selectedEntity.addComponent< StaticMeshComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Skinned Mesh" ) )
            {
                selectedEntity.addComponent< SkinnedMeshComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Rigid Body" ) )
            {
                selectedEntity.addComponent< RigidbodyComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Box Collider" ) )
            {
                selectedEntity.addComponent< BoxColliderComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Sphere Collider" ) )
            {
                selectedEntity.addComponent< SphereColliderComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Capsule Collider" ) )
            {
                selectedEntity.addComponent< CapsuleColliderComponent >();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        drawComponent< TransformComponent >(
            "Transform",
            entity,
            []( auto &transformComponent )
            {
                drawVector3Control( "Position", transformComponent.translation );

                DirectX::XMFLOAT3 rotation_degrees = {};
                rotation_degrees.x = DirectX::XMConvertToDegrees( transformComponent.rotation.x );
                rotation_degrees.y = DirectX::XMConvertToDegrees( transformComponent.rotation.y );
                rotation_degrees.z = DirectX::XMConvertToDegrees( transformComponent.rotation.z );
                drawVector3Control( "Rotation", rotation_degrees );
                transformComponent.rotation.x = DirectX::XMConvertToRadians( rotation_degrees.x );
                transformComponent.rotation.y = DirectX::XMConvertToRadians( rotation_degrees.y );
                transformComponent.rotation.z = DirectX::XMConvertToRadians( rotation_degrees.z );

                drawVector3Control( "Scale", transformComponent.scale, 1.0f );
            },
            false );

        drawComponent< CameraComponent >( "Camera",
            entity,
            []( auto &camera_component )
            {
                ImGui::Checkbox( "Primary Camera", &camera_component.primary );

                const Uint32 projection_type_count = 2;
                const char *projection_type_strs[projection_type_count]{ "Perspective", "Orthographic" };
                const char *current_projection_type_str =
                    projection_type_strs[static_cast< Uint32 >( camera_component.camera.getProjectionType() )];
                if ( ImGui::BeginCombo( "Projection", current_projection_type_str ) )
                {
                    for ( Uint32 i{}; i < projection_type_count; ++i )
                    {
                        bool bSelected = current_projection_type_str == projection_type_strs[i];
                        if ( ImGui::Selectable( projection_type_strs[i], bSelected ) )
                        {
                            current_projection_type_str = projection_type_strs[i];
                            camera_component.camera.setProjectionType(
                                static_cast< SceneCamera::ProjectionType >( i ) );
                        }

                        if ( bSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                switch ( camera_component.camera.getProjectionType() )
                {
                    case SceneCamera::ProjectionType::Perspective:
                    {
                        float fov = camera_component.camera.getFOV();
                        if ( ImGui::DragFloat( "FOV", &fov, 0.03f, 0.03f ) )
                            camera_component.camera.setFOV( fov );

                        float near_plane = camera_component.camera.getPerspectiveNearPlane();
                        if ( ImGui::DragFloat( "Near Plane", &near_plane, 0.03f, 0.03f ) )
                            camera_component.camera.setPerspectiveNearPlane( near_plane );

                        float far_plane = camera_component.camera.getPerspectiveFarPlane();
                        if ( ImGui::DragFloat( "Far Plane", &far_plane, 0.03f, 0.03f ) )
                            camera_component.camera.setPerspectiveFarPlane( far_plane );

                        break;
                    }
                    case SceneCamera::ProjectionType::Orthographic:
                    {
                        float size = camera_component.camera.getSize();
                        if ( ImGui::DragFloat( "Size", &size, 0.03f, 0.03f ) )
                            camera_component.camera.setSize( size );

                        float near_plane = camera_component.camera.getOrthographicNearPlane();
                        if ( ImGui::DragFloat( "Near Plane", &near_plane, 0.03f, 0.03f ) )
                            camera_component.camera.setOrthographicNearPlane( near_plane );

                        float far_plane = camera_component.camera.getOrthographicFarPlane();
                        if ( ImGui::DragFloat( "Far Plane", &far_plane, 0.03f, 0.03f ) )
                            camera_component.camera.setOrthographicFarPlane( far_plane );

                        ImGui::Checkbox( "Fixed Aspect Ratio", &camera_component.fixedAspectRatio );

                        break;
                    }
                }
            } );

        drawComponent< StaticMeshComponent >( "Static Mesh",
            entity,
            []( auto &static_mesh_component )
            {
                ImGui::Button( "Mesh", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( payload )
                    {
                        static_mesh_component.meshes.clear();

                        const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                        std::filesystem::path mesh_path = std::filesystem::path{ assetPath } / path;

                        static_mesh_component.meshes = renderer::MeshLoader::loadStaticMesh( mesh_path.string() );
                        const auto &buffer_layout = static_mesh_component.materials[0]->getBufferLayout();
                        for ( const auto &mesh : static_mesh_component.meshes )
                        {
                            mesh->create( buffer_layout );
                        }
                    }

                    ImGui::EndDragDropTarget();
                }

                for ( const auto &material : static_mesh_component.materials )
                {
                    drawMaterial( material );
                }
            } );

        drawComponent< SkinnedMeshComponent >( "Skinned Mesh",
            entity,
            []( auto &skinned_mesh_component )
            {
                ImGui::Button( "Mesh", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( payload )
                    {
                        skinned_mesh_component.meshes.clear();
                        skinned_mesh_component.animators.clear();

                        const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                        std::filesystem::path mesh_path = std::filesystem::path{ assetPath } / path;

                        skinned_mesh_component.meshes = renderer::MeshLoader::loadSkinnedMesh( mesh_path.string() );
                        const auto &buffer_layout = skinned_mesh_component.materials[0]->getBufferLayout();
                        for ( const auto &mesh : skinned_mesh_component.meshes )
                        {
                            mesh->create( buffer_layout );

                            if ( mesh->hasAnimations() )
                            {
                                renderer::MeshAnimator animator{ mesh };
                                skinned_mesh_component.animators.push_back( animator );
                                skinned_mesh_component.animators.back().setAnimation( 0 );
                            }
                        }
                    }

                    ImGui::EndDragDropTarget();
                }

                for ( const auto &material : skinned_mesh_component.materials )
                {
                    drawMaterial( material );
                }

                for ( auto &animator : skinned_mesh_component.animators )
                {
                    ImGui::Text( "Animator" );
                    if ( ImGui::Button( "Play", { 100.f, 0.0f } ) )
                        animator.play();
                    if ( ImGui::Button( "Pause", { 100.f, 0.0f } ) )
                        animator.pause();
                }
            } );

        drawComponent< RigidbodyComponent >( "Rigidbody",
            entity,
            []( auto &rigidbody_component )
            {
                const Uint32 body_type_count = 2;
                const char *body_type_strs[body_type_count]{ "Static", "Dynamic" };
                const char *current_body_type_str =
                    body_type_strs[static_cast< Uint32 >( rigidbody_component.bodyType )];
                if ( ImGui::BeginCombo( "Body Type", current_body_type_str ) )
                {
                    for ( Uint32 i{}; i < body_type_count; ++i )
                    {
                        bool selected = current_body_type_str == body_type_strs[i];
                        if ( ImGui::Selectable( body_type_strs[i], selected ) )
                        {
                            current_body_type_str = body_type_strs[i];
                            rigidbody_component.bodyType = static_cast< RigidbodyComponent::BodyType >( i );
                        }

                        if ( selected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                const Uint32 collision_detection_count = 2;
                const char *collision_detection_strs[collision_detection_count]{ "Discrete", "Continuous" };
                const char *current_collision_detection_str =
                    collision_detection_strs[static_cast< Uint32 >( rigidbody_component.collisionDetectionType )];
                if ( ImGui::BeginCombo( "Collision Detection Mode", current_collision_detection_str ) )
                {
                    for ( Uint32 i{}; i < collision_detection_count; ++i )
                    {
                        bool selected = current_collision_detection_str == collision_detection_strs[i];
                        if ( ImGui::Selectable( collision_detection_strs[i], selected ) )
                        {
                            current_collision_detection_str = collision_detection_strs[i];
                            rigidbody_component.collisionDetectionType =
                                static_cast< RigidbodyComponent::CollisionDetectionType >( i );
                        }

                        if ( selected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                // TODO: Physics Material

                ImGui::DragFloat( "Mass", &rigidbody_component.mass, 0.03f );
                ImGui::DragFloat( "Linear Drag", &rigidbody_component.linearDrag, 0.01f );
                ImGui::DragFloat( "Angular Drag", &rigidbody_component.angularDrag, 0.01f );
                ImGui::Checkbox( "Disable Gravity", &rigidbody_component.disableGravity );
                ImGui::Checkbox( "Kinematic", &rigidbody_component.kinematic );

                ImGui::Separator();

                ImGui::Checkbox( "Lock Position X", &rigidbody_component.lockPositionX );
                ImGui::Checkbox( "Lock Position Y", &rigidbody_component.lockPositionY );
                ImGui::Checkbox( "Lock Position Z", &rigidbody_component.lockPositionZ );

                ImGui::Separator();

                ImGui::Checkbox( "Lock Rotation X", &rigidbody_component.lockRotationX );
                ImGui::Checkbox( "Lock Rotation Y", &rigidbody_component.lockRotationY );
                ImGui::Checkbox( "Lock Rotation Z", &rigidbody_component.lockRotationZ );
            } );

        drawComponent< BoxColliderComponent >( "Box Collider",
            entity,
            []( auto &box_collider_component )
            {
                ImGui::DragFloat3( "Size", &box_collider_component.size.x, 0.03f );
                ImGui::DragFloat3( "Offset", &box_collider_component.offset.x, 0.03f );
                ImGui::Checkbox( "Trigger", &box_collider_component.trigger );
                ImGui::Checkbox( "Show Collider Bounds", &box_collider_component.showColliderBounds );

                // TODO: Physics Material
            } );

        drawComponent< SphereColliderComponent >( "Sphere Collider",
            entity,
            []( auto &sphere_collider_component )
            {
                ImGui::DragFloat( "Radius", &sphere_collider_component.radius, 0.03f );
                ImGui::Checkbox( "Trigger", &sphere_collider_component.trigger );
                ImGui::Checkbox( "Show Collider Bounds", &sphere_collider_component.showColliderBounds );
            } );

        drawComponent< CapsuleColliderComponent >( "Capsule Collider",
            entity,
            []( auto &capsule_collider_component )
            {
                ImGui::DragFloat( "Radius", &capsule_collider_component.radius, 0.03f );
                ImGui::DragFloat( "Height", &capsule_collider_component.height, 0.03f );
                ImGui::Checkbox( "Trigger", &capsule_collider_component.trigger );
                ImGui::Checkbox( "Show Collider Bounds", &capsule_collider_component.showColliderBounds );
            } );
    }

    template < typename ComponentType, typename UIFunction >
    void SceneHierarchyPanel::drawComponent( const std::string &label,
        Entity entity,
        UIFunction ui_function,
        bool removeable )
    {
        if ( !entity.hasComponent< ComponentType >() )
            return;

        auto &component = entity.getComponent< ComponentType >();
        const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                                   ImGuiTreeNodeFlags_AllowItemOverlap |
                                                   ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

        ImVec2 content_region_available = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4.f, 4.f } );

        const float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImGui::Separator();

        bool open = ImGui::TreeNodeEx( ( void * )typeid( ComponentType ).hash_code(), tree_node_flags, label.c_str() );
        ImGui::PopStyleVar();

        ImGui::SameLine( content_region_available.x - line_height * 0.5f );
        if ( ImGui::Button( "+", ImVec2{ line_height, line_height } ) )
        {
            ImGui::OpenPopup( "ComponentSettings" );
        }

        bool remove_component = false;
        if ( ImGui::BeginPopup( "ComponentSettings" ) )
        {
            if ( ImGui::MenuItem( "Remove Component" ) )
                remove_component = true;

            ImGui::EndPopup();
        }

        if ( open )
        {
            ui_function( component );
            ImGui::TreePop();
        }

        if ( removeable && remove_component )
            entity.removeComponent< ComponentType >();
    }

    void SceneHierarchyPanel::drawMaterial( const Ref< renderer::Material > &material )
    {
        const Ref< renderer::Shader > &shader = material->getShader();
        ImGui::Text( "Material" );

        ImGui::Button( shader->getName().c_str(), { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( payload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                std::filesystem::path shader_path = std::filesystem::path{ assetPath } / path;
                material->setShader( renderer::Shader::create( shader_path.string() ) );
            }

            ImGui::EndDragDropTarget();
        }

        const auto &float_values{ material->getFloatValues() };
        for ( const auto &pair : float_values )
        {
            float value = material->getFloatValue( pair.first );
            ImGui::DragFloat( pair.first.c_str(), &value, 0.03f );
            material->setFloatValue( pair.first, value );
        }

        const auto &int_values{ material->getIntValues() };
        for ( const auto &pair : int_values )
        {
            int value = material->getIntValue( pair.first );
            ImGui::DragInt( pair.first.c_str(), &value, 0.03f );
            material->setIntValue( pair.first, value );
        }

        const auto &bool_values{ material->getBoolValues() };
        for ( const auto &pair : bool_values )
        {
            bool value = material->getBoolValue( pair.first );
            ImGui::Checkbox( pair.first.c_str(), &value );
            material->setBoolValue( pair.first, value );
        }

        const auto &float2_values{ material->getFloat2Values() };
        for ( const auto &pair : float2_values )
        {
            DirectX::XMFLOAT2 value = material->getFloat2Value( pair.first );
            ImGui::DragFloat2( pair.first.c_str(), &value.x, 0.03f );
            material->setFloat2Value( pair.first, value );
        }

        const auto &float3_values{ material->getFloat3Values() };
        for ( const auto &pair : float3_values )
        {
            DirectX::XMFLOAT3 value = material->getFloat3Value( pair.first );
            ImGui::DragFloat3( pair.first.c_str(), &value.x, 0.03f );
            material->setFloat3Value( pair.first, value );
        }

        const auto &texture_2d_values{ material->getTexture2DValues() };
        for ( const auto &pair : texture_2d_values )
        {
            ImGui::Button( pair.first.c_str(), { 100.f, 0.0f } );
            if ( ImGui::BeginDragDropTarget() )
            {
                const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                if ( payload )
                {
                    const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                    std::filesystem::path texture_path = std::filesystem::path{ assetPath } / path;
                    material->setTexture2D( pair.first, renderer::Texture2D::create( texture_path.string() ) );
                }

                ImGui::EndDragDropTarget();
            }
        }
    }
}