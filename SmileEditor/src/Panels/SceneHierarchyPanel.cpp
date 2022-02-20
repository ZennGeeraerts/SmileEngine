#include "SceneHierarchyPanel.h"

#include <Libs/ImGui/imgui.h>
#include <Libs/ImGui/imgui_internal.h>

#include "SmileEngine/Scene/Components.h"
#include "ContentBrowserPanel.h"

namespace smile
{
    extern const std::filesystem::path g_ResourcePath;

    SceneHierarchyPanel::SceneHierarchyPanel( const Ref< Scene > &pScene )
    {
        SetContext( pScene );
    }

    void SceneHierarchyPanel::SetContext( const Ref< Scene > &pScene )
    {
        m_pContext = pScene;
        m_SelectedEntity = {};
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin( "Scene Hierarchy" );

        if ( m_pContext )
        {
            m_pContext->m_Registry.each(
                [&]( auto entityID )
                {
                    Entity entity{ entityID, m_pContext.get() };
                    DrawEntityNode( entity );
                } );

            // Deselect entities
            if ( ImGui::IsMouseDown( 0 ) && ImGui::IsWindowHovered() )
                m_SelectedEntity = {};

            // Right click on blank space
            if ( ImGui::BeginPopupContextWindow( 0, 1, false ) )
            {
                if ( ImGui::MenuItem( "Create Empty Entity" ) )
                    m_pContext->CreateEntity();

                ImGui::EndPopup();
            }
        }

        ImGui::End();

        // Inspector Panel
        ImGui::Begin( "Inspector" );

        // Add Components
        if ( m_SelectedEntity )
        {
            DrawComponents( m_SelectedEntity );
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode( Entity entity )
    {
        auto &tag = entity.GetComponent< TagComponent >().m_Tag;

        const ImGuiTreeNodeFlags flags = ( ( m_SelectedEntity == entity ) ? ImGuiTreeNodeFlags_Selected : 0 ) |
                                         ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool bNodeExpanded = ImGui::TreeNodeEx( ( void * )( uint64_t )( uint32_t )entity, flags, tag.c_str() );
        if ( ImGui::IsItemClicked() )
        {
            m_SelectedEntity = entity;
        }

        bool bEntityDeleted = false;
        if ( ImGui::BeginPopupContextItem() )
        {
            if ( ImGui::MenuItem( "Delete Entity" ) )
                bEntityDeleted = true;

            ImGui::EndPopup();
        }

        if ( bNodeExpanded )
        {
            ImGui::TreePop();
        }

        if ( bEntityDeleted )
        {
            m_pContext->DestroyEntity( entity );
            if ( m_SelectedEntity == entity )
                m_SelectedEntity = {};
        }
    }

    void SceneHierarchyPanel::DrawVector3Control( const std::string &label,
        DirectX::XMFLOAT3 &value,
        float resetValue,
        float columnWidth )
    {
        ImGuiIO &io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[1];

        ImGui::PushID( label.c_str() );

        ImGui::Columns( 2 );

        ImGui::SetColumnWidth( 0, columnWidth );
        ImGui::Text( label.c_str() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

        // X
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f } );
        ImGui::PushFont( boldFont );

        if ( ImGui::Button( "X", buttonSize ) )
            value.x = resetValue;

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
        ImGui::PushFont( boldFont );

        if ( ImGui::Button( "Y", buttonSize ) )
            value.y = resetValue;

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
        ImGui::PushFont( boldFont );

        if ( ImGui::Button( "Z", buttonSize ) )
            value.z = resetValue;

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

    void SceneHierarchyPanel::DrawComponents( Entity entity )
    {
        if ( entity.HasComponent< TagComponent >() )
        {
            auto &tag = entity.GetComponent< TagComponent >().m_Tag;

            char tagBuffer[256];
            memset( tagBuffer, 0, sizeof( tagBuffer ) );
            strcpy_s( tagBuffer, sizeof( tagBuffer ), tag.c_str() );

            if ( ImGui::InputText( "##Tag", tagBuffer, sizeof( tagBuffer ) ) )
            {
                tag = std::string{ tagBuffer };
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
                m_SelectedEntity.AddComponent< CameraComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Static Mesh" ) )
            {
                m_SelectedEntity.AddComponent< StaticMeshComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Skinned Mesh" ) )
            {
                m_SelectedEntity.AddComponent< SkinnedMeshComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Rigid Body" ) )
            {
                m_SelectedEntity.AddComponent< RigidbodyComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Box Collider" ) )
            {
                m_SelectedEntity.AddComponent< BoxColliderComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Sphere Collider" ) )
            {
                m_SelectedEntity.AddComponent< SphereColliderComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Capsule Collider" ) )
            {
                m_SelectedEntity.AddComponent< CapsuleColliderComponent >();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponent< TransformComponent >(
            "Transform",
            entity,
            []( auto &transformComponent )
            {
                DrawVector3Control( "Position", transformComponent.m_Translation );

                DirectX::XMFLOAT3 rotationDegrees = {};
                rotationDegrees.x = DirectX::XMConvertToDegrees( transformComponent.m_Rotation.x );
                rotationDegrees.y = DirectX::XMConvertToDegrees( transformComponent.m_Rotation.y );
                rotationDegrees.z = DirectX::XMConvertToDegrees( transformComponent.m_Rotation.z );
                DrawVector3Control( "Rotation", rotationDegrees );
                transformComponent.m_Rotation.x = DirectX::XMConvertToRadians( rotationDegrees.x );
                transformComponent.m_Rotation.y = DirectX::XMConvertToRadians( rotationDegrees.y );
                transformComponent.m_Rotation.z = DirectX::XMConvertToRadians( rotationDegrees.z );

                DrawVector3Control( "Scale", transformComponent.m_Scale, 1.0f );
            },
            false );

        DrawComponent< CameraComponent >( "Camera",
            entity,
            []( auto &cameraComponent )
            {
                ImGui::Checkbox( "Primary Camera", &cameraComponent.m_bPrimary );

                const uint32_t projectionTypeCount = 2;
                const char *pProjectionTypeStrs[projectionTypeCount]{ "Perspective", "Orthographic" };
                const char *pCurrentProjectionTypeStr =
                    pProjectionTypeStrs[static_cast< uint32_t >( cameraComponent.m_Camera.GetProjectionType() )];
                if ( ImGui::BeginCombo( "Projection", pCurrentProjectionTypeStr ) )
                {
                    for ( uint32_t i{}; i < projectionTypeCount; ++i )
                    {
                        bool bSelected = pCurrentProjectionTypeStr == pProjectionTypeStrs[i];
                        if ( ImGui::Selectable( pProjectionTypeStrs[i], bSelected ) )
                        {
                            pCurrentProjectionTypeStr = pProjectionTypeStrs[i];
                            cameraComponent.m_Camera.SetProjectionType( static_cast< SceneCamera::ProjectionType >( i ) );
                        }

                        if ( bSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                switch ( cameraComponent.m_Camera.GetProjectionType() )
                {
                    case SceneCamera::ProjectionType::Perspective:
                    {
                        float fov = cameraComponent.m_Camera.GetFOV();
                        if ( ImGui::DragFloat( "FOV", &fov, 0.03f, 0.03f ) )
                            cameraComponent.m_Camera.SetFOV( fov );

                        float nearPlane = cameraComponent.m_Camera.GetPerspectiveNearPlane();
                        if ( ImGui::DragFloat( "Near Plane", &nearPlane, 0.03f, 0.03f ) )
                            cameraComponent.m_Camera.SetPerspectiveNearPlane( nearPlane );

                        float farPlane = cameraComponent.m_Camera.GetPerspectiveFarPlane();
                        if ( ImGui::DragFloat( "Far Plane", &farPlane, 0.03f, 0.03f ) )
                            cameraComponent.m_Camera.SetPerspectiveFarPlane( farPlane );

                        break;
                    }
                    case SceneCamera::ProjectionType::Orthographic:
                    {
                        float size = cameraComponent.m_Camera.GetSize();
                        if ( ImGui::DragFloat( "Size", &size, 0.03f, 0.03f ) )
                            cameraComponent.m_Camera.SetSize( size );

                        float nearPlane = cameraComponent.m_Camera.GetOrthographicNearPlane();
                        if ( ImGui::DragFloat( "Near Plane", &nearPlane, 0.03f, 0.03f ) )
                            cameraComponent.m_Camera.SetOrthographicNearPlane( nearPlane );

                        float farPlane = cameraComponent.m_Camera.GetOrthographicFarPlane();
                        if ( ImGui::DragFloat( "Far Plane", &farPlane, 0.03f, 0.03f ) )
                            cameraComponent.m_Camera.SetOrthographicFarPlane( farPlane );

                        ImGui::Checkbox( "Fixed Aspect Ratio", &cameraComponent.m_bFixedAspectRatio );

                        break;
                    }
                }
            } );

        DrawComponent< StaticMeshComponent >( "Static Mesh",
            entity,
            []( auto &staticMeshComponent )
            {
                ImGui::Button( "Mesh", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( pPayload )
                    {
                        staticMeshComponent.m_pMeshes.clear();

                        const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                        std::filesystem::path meshPath = std::filesystem::path{ g_ResourcePath } / path;

                        staticMeshComponent.m_pMeshes = MeshLoader::LoadStaticMesh( meshPath.string() );
                        const auto &bufferLayout = staticMeshComponent.m_pMaterials[0]->GetBufferLayout();
                        for ( const auto &pMesh : staticMeshComponent.m_pMeshes )
                        {
                            pMesh->Create( bufferLayout );
                        }
                    }

                    ImGui::EndDragDropTarget();
                }

                for ( const auto &pMaterial : staticMeshComponent.m_pMaterials )
                {
                    DrawMaterial( pMaterial );
                }
            } );

        DrawComponent< SkinnedMeshComponent >( "Skinned Mesh",
            entity,
            []( auto &skinnedMeshComponent )
            {
                ImGui::Button( "Mesh", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( pPayload )
                    {
                        skinnedMeshComponent.m_pMeshes.clear();
                        skinnedMeshComponent.m_Animators.clear();

                        const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                        std::filesystem::path meshPath = std::filesystem::path{ g_ResourcePath } / path;

                        skinnedMeshComponent.m_pMeshes = MeshLoader::LoadSkinnedMesh( meshPath.string() );
                        const auto &bufferLayout = skinnedMeshComponent.m_pMaterials[0]->GetBufferLayout();
                        for ( const auto &pMesh : skinnedMeshComponent.m_pMeshes )
                        {
                            pMesh->Create( bufferLayout );

                            if ( pMesh->HasAnimations() )
                            {
                                MeshAnimator animator{ pMesh };
                                skinnedMeshComponent.m_Animators.push_back( animator );
                                skinnedMeshComponent.m_Animators.back().SetAnimation( 0 );
                            }
                        }
                    }

                    ImGui::EndDragDropTarget();
                }

                for ( const auto &pMaterial : skinnedMeshComponent.m_pMaterials )
                {
                    DrawMaterial( pMaterial );
                }

                for ( auto &animator : skinnedMeshComponent.m_Animators )
                {
                    ImGui::Text( "Animator" );
                    if ( ImGui::Button( "Play", { 100.f, 0.0f } ) )
                        animator.Play();
                    if ( ImGui::Button( "Pause", { 100.f, 0.0f } ) )
                        animator.Pause();
                }
            } );

        DrawComponent< RigidbodyComponent >( "Rigidbody",
            entity,
            []( auto &rigidbodyComponent )
            {
                const uint32_t bodyTypeCount = 2;
                const char *bodyTypeStrs[bodyTypeCount]{ "Static", "Dynamic" };
                const char *currentBodyTypeStr = bodyTypeStrs[static_cast< uint32_t >( rigidbodyComponent.m_BodyType )];
                if ( ImGui::BeginCombo( "Body Type", currentBodyTypeStr ) )
                {
                    for ( uint32_t i{}; i < bodyTypeCount; ++i )
                    {
                        bool bSelected = currentBodyTypeStr == bodyTypeStrs[i];
                        if ( ImGui::Selectable( bodyTypeStrs[i], bSelected ) )
                        {
                            currentBodyTypeStr = bodyTypeStrs[i];
                            rigidbodyComponent.m_BodyType = static_cast< RigidbodyComponent::BodyType >( i );
                        }

                        if ( bSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                const Uint32 collisionDetectionCount = 2;
                const char *pCollisionDetectionStrs[collisionDetectionCount]{ "Discrete", "Continuous" };
                const char *pCurrentCollisionDetectionStr =
                    pCollisionDetectionStrs[static_cast< uint32_t >( rigidbodyComponent.m_CollisionDetectionType )];
                if ( ImGui::BeginCombo( "Collision Detection Mode", pCurrentCollisionDetectionStr ) )
                {
                    for ( Uint32 i{}; i < collisionDetectionCount; ++i )
                    {
                        bool bSelected = pCurrentCollisionDetectionStr == pCollisionDetectionStrs[i];
                        if ( ImGui::Selectable( pCollisionDetectionStrs[i], bSelected ) )
                        {
                            pCurrentCollisionDetectionStr = pCollisionDetectionStrs[i];
                            rigidbodyComponent.m_CollisionDetectionType =
                                static_cast< RigidbodyComponent::CollisionDetectionType >( i );
                        }

                        if ( bSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                // TODO: Physics Material

                ImGui::DragFloat( "Mass", &rigidbodyComponent.m_Mass, 0.03f );
                ImGui::DragFloat( "Linear Drag", &rigidbodyComponent.m_LinearDrag, 0.01f );
                ImGui::DragFloat( "Angular Drag", &rigidbodyComponent.m_AngularDrag, 0.01f );
                ImGui::Checkbox( "Disable Gravity", &rigidbodyComponent.m_bDisableGravity );
                ImGui::Checkbox( "Kinematic", &rigidbodyComponent.m_bKinematic );

                ImGui::Separator();

                ImGui::Checkbox( "Lock Position X", &rigidbodyComponent.m_bLockPositionX );
                ImGui::Checkbox( "Lock Position Y", &rigidbodyComponent.m_bLockPositionY );
                ImGui::Checkbox( "Lock Position Z", &rigidbodyComponent.m_bLockPositionZ );

                ImGui::Separator();

                ImGui::Checkbox( "Lock Rotation X", &rigidbodyComponent.m_bLockRotationX );
                ImGui::Checkbox( "Lock Rotation Y", &rigidbodyComponent.m_bLockRotationY );
                ImGui::Checkbox( "Lock Rotation Z", &rigidbodyComponent.m_bLockRotationZ );
            } );

        DrawComponent< BoxColliderComponent >( "Box Collider",
            entity,
            []( auto &boxColliderComponent )
            {
                ImGui::DragFloat3( "Size", &boxColliderComponent.m_Size.x, 0.03f );
                ImGui::DragFloat3( "Offset", &boxColliderComponent.m_Offset.x, 0.03f );
                ImGui::Checkbox( "Trigger", &boxColliderComponent.m_bTrigger );
                ImGui::Checkbox( "Show Collider Bounds", &boxColliderComponent.m_bShowColliderBounds );

                // TODO: Physics Material
            } );

        DrawComponent< SphereColliderComponent >( "Sphere Collider",
            entity,
            []( auto &sphereColliderComponent )
            {
                ImGui::DragFloat( "Radius", &sphereColliderComponent.m_Radius, 0.03f );
                ImGui::Checkbox( "Trigger", &sphereColliderComponent.m_bTrigger );
                ImGui::Checkbox( "Show Collider Bounds", &sphereColliderComponent.m_bShowColliderBounds );
            } );

        DrawComponent< CapsuleColliderComponent >( "Capsule Collider",
            entity,
            []( auto &capsuleColliderComponent )
            {
                ImGui::DragFloat( "Radius", &capsuleColliderComponent.m_Radius, 0.03f );
                ImGui::DragFloat( "Height", &capsuleColliderComponent.m_Height, 0.03f );
                ImGui::Checkbox( "Trigger", &capsuleColliderComponent.m_bTrigger );
                ImGui::Checkbox( "Show Collider Bounds", &capsuleColliderComponent.m_bShowColliderBounds );
            } );
    }

    template < typename ComponentType, typename UIFunction >
    void SceneHierarchyPanel::DrawComponent( const std::string &label,
        Entity entity,
        UIFunction uiFunction,
        bool bRemoveable )
    {
        if ( !entity.HasComponent< ComponentType >() )
            return;

        auto &component = entity.GetComponent< ComponentType >();
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                                 ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding |
                                                 ImGuiTreeNodeFlags_SpanAvailWidth;

        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4.f, 4.f } );

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImGui::Separator();

        bool bOpen = ImGui::TreeNodeEx( ( void * )typeid( ComponentType ).hash_code(), treeNodeFlags, label.c_str() );
        ImGui::PopStyleVar();

        ImGui::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( ImGui::Button( "+", ImVec2{ lineHeight, lineHeight } ) )
        {
            ImGui::OpenPopup( "ComponentSettings" );
        }

        bool bRemoveComponent = false;
        if ( ImGui::BeginPopup( "ComponentSettings" ) )
        {
            if ( ImGui::MenuItem( "Remove Component" ) )
                bRemoveComponent = true;

            ImGui::EndPopup();
        }

        if ( bOpen )
        {
            uiFunction( component );
            ImGui::TreePop();
        }

        if ( bRemoveable && bRemoveComponent )
            entity.RemoveComponent< ComponentType >();
    }

    void SceneHierarchyPanel::DrawMaterial( const Ref< Material > &pMaterial )
    {
        // Albedo
        ImGui::Button( "Albedo Map", { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( payload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                std::filesystem::path texturePath = std::filesystem::path{ g_ResourcePath } / path;
                pMaterial->SetUseAlbedoMap( true );
                pMaterial->SetAlbedo( Texture2D::Create( texturePath.string() ) );
            }

            ImGui::EndDragDropTarget();
        }

        auto albedoColor = pMaterial->GetAlbedoColor();
        ImGui::ColorPicker3( "Albedo Color", reinterpret_cast< float * >( &albedoColor ) );
        pMaterial->SetAlbedo( albedoColor );

        // Metalness
        ImGui::Button( "Metalness Map", { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( payload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                std::filesystem::path texturePath = std::filesystem::path{ g_ResourcePath } / path;
                pMaterial->SetUseMetalnessMap( true );
                pMaterial->SetMetalness( Texture2D::Create( texturePath.string() ) );
            }

            ImGui::EndDragDropTarget();
        }

        auto metalnessValue = pMaterial->GetMetalness();
        ImGui::SliderFloat( "Metalness", &metalnessValue, 0, 1 );
        pMaterial->SetMetalness( metalnessValue );

        // Roughness
        ImGui::Button( "Roughness Map", { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( payload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                std::filesystem::path texturePath = std::filesystem::path{ g_ResourcePath } / path;
                pMaterial->SetUseRoughnessMap( true );
                pMaterial->SetRoughness( Texture2D::Create( texturePath.string() ) );
            }

            ImGui::EndDragDropTarget();
        }

        auto roughnessValue = pMaterial->GetRoughness();
        ImGui::SliderFloat( "Roughness", &roughnessValue, 0, 1 );
        pMaterial->SetRoughness( roughnessValue );

        // Normal
        ImGui::Button( "Normal Map", { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( payload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( payload->Data );
                std::filesystem::path texturePath = std::filesystem::path{ g_ResourcePath } / path;
                pMaterial->SetUseNormalMap( true );
                pMaterial->SetNormalMap( Texture2D::Create( texturePath.string() ) );
            }

            ImGui::EndDragDropTarget();
        }

        // AO
        ImGui::Button( "Ambient Occlusion Map", { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( pPayload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                std::filesystem::path texturePath = std::filesystem::path{ g_ResourcePath } / path;
                pMaterial->SetUseAOMap( true );
                pMaterial->SetAOMap( Texture2D::Create( texturePath.string() ) );
            }

            ImGui::EndDragDropTarget();
        }
    }
}