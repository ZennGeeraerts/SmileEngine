/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "scene_hierarchy_panel.h"

#include "smile_engine/scene/components.h"
#include "smile_engine/scripting/script_engine.h"
#include "smile_engine/graphic/mesh/mesh.h"
#include "smile_engine/graphic/mesh/mesh_factory.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace smile::scene
{
    SceneHierarchyPanel::SceneHierarchyPanel( Ref< Scene > pScene )
    {
        SetContext( pScene );
    }

    void SceneHierarchyPanel::SetContext( Ref< Scene > pScene )
    {
        m_pContext = pScene;
        m_SelectedEntity = {};
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin( "Scene Hierarchy" );

        if ( m_pContext )
        {
            m_pContext->m_ECSEngine.Each(
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
        auto &tag = entity.GetComponent< TagComponent >().Tag;

        const ImGuiTreeNodeFlags flags = ( ( m_SelectedEntity == entity ) ? ImGuiTreeNodeFlags_Selected : 0 ) |
                                         ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool isNodeExpanded = ImGui::TreeNodeEx( ( void * )( Uint64 )entity, flags, tag.c_str() );
        if ( ImGui::IsItemClicked() )
        {
            m_SelectedEntity = entity;
        }

        bool isEntityDeleted = false;
        if ( ImGui::BeginPopupContextItem() )
        {
            if ( ImGui::MenuItem( "Delete Entity" ) )
                isEntityDeleted = true;

            ImGui::EndPopup();
        }

        if ( isNodeExpanded )
        {
            ImGui::TreePop();
        }

        if ( isEntityDeleted )
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
            auto &tag = entity.GetComponent< TagComponent >().Tag;

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

            if ( ImGui::MenuItem( "Script" ) )
            {
                m_SelectedEntity.AddComponent< ScriptComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Mesh Renderer" ) )
            {
                m_SelectedEntity.AddComponent< MeshRendererComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Skinned Mesh Renderer" ) )
            {
                m_SelectedEntity.AddComponent< SkinnedMeshRendererComponent >();
                ImGui::CloseCurrentPopup();
            }

            if ( ImGui::MenuItem( "Animator" ) )
            {
                m_SelectedEntity.AddComponent< AnimatorComponent >();
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
                DrawVector3Control( "Position", transformComponent.Translation );

                DirectX::XMFLOAT3 rotationDegrees = {};
                rotationDegrees.x = DirectX::XMConvertToDegrees( transformComponent.Rotation.x );
                rotationDegrees.y = DirectX::XMConvertToDegrees( transformComponent.Rotation.y );
                rotationDegrees.z = DirectX::XMConvertToDegrees( transformComponent.Rotation.z );
                DrawVector3Control( "Rotation", rotationDegrees );
                transformComponent.Rotation.x = DirectX::XMConvertToRadians( rotationDegrees.x );
                transformComponent.Rotation.y = DirectX::XMConvertToRadians( rotationDegrees.y );
                transformComponent.Rotation.z = DirectX::XMConvertToRadians( rotationDegrees.z );

                DrawVector3Control( "Scale", transformComponent.Scale, 1.0f );
            },
            false );

        DrawComponent< CameraComponent >( "Camera",
            entity,
            []( auto &cameraComponent )
            {
                ImGui::Checkbox( "Primary Camera", &cameraComponent.IsPrimary );

                const Uint32 projectionTypeCount = 2;
                const char *projectionTypeStrs[projectionTypeCount]{ "Perspective", "Orthographic" };
                const char *currentProjectionTypeStr =
                    projectionTypeStrs[static_cast< Uint32 >( cameraComponent.Camera.GetProjectionType() )];
                if ( ImGui::BeginCombo( "Projection", currentProjectionTypeStr ) )
                {
                    for ( Uint32 i{}; i < projectionTypeCount; ++i )
                    {
                        bool isSelected = currentProjectionTypeStr == projectionTypeStrs[i];
                        if ( ImGui::Selectable( projectionTypeStrs[i], isSelected ) )
                        {
                            currentProjectionTypeStr = projectionTypeStrs[i];
                            cameraComponent.Camera.SetProjectionType( static_cast< SceneCamera::ProjectionType >( i ) );
                        }

                        if ( isSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                switch ( cameraComponent.Camera.GetProjectionType() )
                {
                    case SceneCamera::ProjectionType::Perspective:
                    {
                        float fov = cameraComponent.Camera.GetFOV();
                        if ( ImGui::DragFloat( "FOV", &fov, 0.03f, 0.03f ) )
                            cameraComponent.Camera.SetFOV( fov );

                        float nearPlane = cameraComponent.Camera.GetPerspectiveNearPlane();
                        if ( ImGui::DragFloat( "Near Plane", &nearPlane, 0.03f, 0.03f ) )
                            cameraComponent.Camera.SetPerspectiveNearPlane( nearPlane );

                        float farPlane = cameraComponent.Camera.GetPerspectiveFarPlane();
                        if ( ImGui::DragFloat( "Far Plane", &farPlane, 0.03f, 0.03f ) )
                            cameraComponent.Camera.SetPerspectiveFarPlane( farPlane );

                        break;
                    }
                    case SceneCamera::ProjectionType::Orthographic:
                    {
                        float size = cameraComponent.Camera.GetSize();
                        if ( ImGui::DragFloat( "Size", &size, 0.03f, 0.03f ) )
                            cameraComponent.Camera.SetSize( size );

                        float nearPlane = cameraComponent.Camera.GetOrthographicNearPlane();
                        if ( ImGui::DragFloat( "Near Plane", &nearPlane, 0.03f, 0.03f ) )
                            cameraComponent.Camera.SetOrthographicNearPlane( nearPlane );

                        float farPlane = cameraComponent.Camera.GetOrthographicFarPlane();
                        if ( ImGui::DragFloat( "Far Plane", &farPlane, 0.03f, 0.03f ) )
                            cameraComponent.Camera.SetOrthographicFarPlane( farPlane );

                        ImGui::Checkbox( "Fixed Aspect Ratio", &cameraComponent.HasFixedAspectRatio );

                        break;
                    }
                }
            } );

        DrawComponent< ScriptComponent >( "Script",
            entity,
            []( auto &scriptComponent )
            {
                bool scriptClassExists = scripting::ScriptEngine::EntityClassExists( scriptComponent.ClassName );

                if ( !scriptClassExists )
                    ImGui::PushStyleColor( ImGuiCol_Text, ImVec4{ 0.9f, 0.2f, 0.3f, 1.0f } );

                static char buffer[64];
                strcpy_s( buffer, scriptComponent.ClassName.c_str() );
                if ( ImGui::InputText( "Class", buffer, sizeof( buffer ) ) )
                {
                    scriptComponent.ClassName = buffer;
                }

                if ( !scriptClassExists )
                    ImGui::PopStyleColor();
            } );

        DrawComponent< MeshRendererComponent >( "Mesh Renderer",
            entity,
            []( auto &meshRendererComponent )
            {
                static Ref< graphic::Model > pLoadedModel = nullptr;

                ImGui::Button( "Mesh", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( pPayload )
                    {
                        const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                        std::filesystem::path modelPath = std::filesystem::path{ path };

                        pLoadedModel = graphic::ModelLoader::LoadModel( modelPath.string() );
                    }

                    ImGui::EndDragDropTarget();
                }

                if ( pLoadedModel )
                    ImGui::OpenPopup( "SetMeshPopup" );

                if ( ImGui::BeginPopup( "SetMeshPopup" ) )
                {
                    static auto pSelectedMesh = pLoadedModel->GetMeshFilter( 0 );

                    if ( ImGui::BeginCombo( "Mesh", pSelectedMesh->GetName().c_str() ) )
                    {
                        for ( Uint32 i{}; i < pLoadedModel->GetMeshCount(); ++i )
                        {
                            auto pMeshFilter = pLoadedModel->GetMeshFilter( i );
                            bool isSelected = pMeshFilter == pSelectedMesh;

                            if ( ImGui::Selectable( pMeshFilter->GetName().c_str(), isSelected ) )
                            {
                                pSelectedMesh = pMeshFilter;
                                meshRendererComponent.pMesh.reset();
                                meshRendererComponent.pMesh = graphic::MeshFactory::CreateMesh(
                                    pMeshFilter, meshRendererComponent.pMaterial->GetBufferLayout() );

                                meshRendererComponent.pModel = pLoadedModel;
                                meshRendererComponent.MeshIndex = i;
                            }

                            if ( isSelected )
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }

                    if ( ImGui::Button( "Set Mesh", ImVec2{ 100.f, 0.f } ) )
                    {
                        pLoadedModel = nullptr;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }

                DrawMaterial( meshRendererComponent.pMaterial );
            } );

        DrawComponent< SkinnedMeshRendererComponent >( "Skinned Mesh Renderer",
            entity,
            []( auto &skinnedMeshRendererComponent )
            {
                static Ref< graphic::Model > pLoadedModel = nullptr;

                ImGui::Button( "Skinned Mesh", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( pPayload )
                    {
                        skinnedMeshRendererComponent.pSkinnedMesh.reset();

                        const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                        std::filesystem::path modelPath = std::filesystem::path{ path };

                        pLoadedModel = graphic::ModelLoader::LoadModel( modelPath.string() );
                    }

                    ImGui::EndDragDropTarget();
                }

                if ( pLoadedModel )
                    ImGui::OpenPopup( "SetMeshPopup" );

                if ( ImGui::BeginPopup( "SetMeshPopup" ) )
                {
                    static auto pSelectedMesh = pLoadedModel->GetSkinnedMeshFilter( 0 );

                    if ( ImGui::BeginCombo( "Skinned Mesh", pSelectedMesh->GetName().c_str() ) )
                    {
                        for ( Uint32 i{}; i < pLoadedModel->GetSkinnedMeshCount(); ++i )
                        {
                            auto pSkinnedMeshFilter = pLoadedModel->GetSkinnedMeshFilter( i );
                            bool isSelected = pSkinnedMeshFilter == pSelectedMesh;

                            if ( ImGui::Selectable( pSkinnedMeshFilter->GetName().c_str(), isSelected ) )
                            {
                                pSelectedMesh = pSkinnedMeshFilter;
                                skinnedMeshRendererComponent.pSkinnedMesh.reset();
                                skinnedMeshRendererComponent.pSkinnedMesh = graphic::MeshFactory::CreateSkinnedMesh(
                                    pSkinnedMeshFilter, skinnedMeshRendererComponent.pMaterial->GetBufferLayout() );

                                skinnedMeshRendererComponent.pModel = pLoadedModel;
                                skinnedMeshRendererComponent.MeshIndex = i;
                            }

                            if ( isSelected )
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }

                    if ( ImGui::Button( "Set Skinned Mesh", ImVec2{ 100.f, 0.f } ) )
                    {
                        pLoadedModel = nullptr;
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }

                DrawMaterial( skinnedMeshRendererComponent.pMaterial );
            } );

        DrawComponent< AnimatorComponent >( "Animator",
            entity,
            []( auto &animatorComponent ) 
            {
                ImGui::Button( "Animations", { 100.f, 0.0f } );
                if ( ImGui::BeginDragDropTarget() )
                {
                    const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                    if ( pPayload )
                    {
                        const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                        std::filesystem::path modelPath = std::filesystem::path{ path };

                        Ref< graphic::Model > pLoadedModel = graphic::ModelLoader::LoadModel( modelPath.string() );
                        animatorComponent.pAnimationClips = pLoadedModel->GetAnimationClips();
                        animatorComponent.CurrentClipIndex = 0;
                        animatorComponent.pModel = pLoadedModel;
                    }

                    ImGui::EndDragDropTarget();
                }
            } );

        DrawComponent< RigidbodyComponent >( "Rigidbody",
            entity,
            []( auto &rigidbodyComponent )
            {
                const Uint32 bodyTypeCount = 2;
                const char *bodyTypeStrs[bodyTypeCount]{ "Static", "Dynamic" };
                const char *currentBodyTypeStr = bodyTypeStrs[static_cast< Uint32 >( rigidbodyComponent.Type )];
                if ( ImGui::BeginCombo( "Body Type", currentBodyTypeStr ) )
                {
                    for ( Uint32 i{}; i < bodyTypeCount; ++i )
                    {
                        bool isSelected = currentBodyTypeStr == bodyTypeStrs[i];
                        if ( ImGui::Selectable( bodyTypeStrs[i], isSelected ) )
                        {
                            currentBodyTypeStr = bodyTypeStrs[i];
                            rigidbodyComponent.Type = static_cast< RigidbodyComponent::BodyType >( i );
                        }

                        if ( isSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                const Uint32 collisionDetectionCount = 2;
                const char *collisionDetectionStrs[collisionDetectionCount]{ "Discrete", "Continuous" };
                const char *currentCollisionDetectionStr =
                    collisionDetectionStrs[static_cast< Uint32 >( rigidbodyComponent.CollisionDetection )];
                if ( ImGui::BeginCombo( "Collision Detection Mode", currentCollisionDetectionStr ) )
                {
                    for ( Uint32 i{}; i < collisionDetectionCount; ++i )
                    {
                        bool isSelected = currentCollisionDetectionStr == collisionDetectionStrs[i];
                        if ( ImGui::Selectable( collisionDetectionStrs[i], isSelected ) )
                        {
                            currentCollisionDetectionStr = collisionDetectionStrs[i];
                            rigidbodyComponent.CollisionDetection =
                                static_cast< RigidbodyComponent::CollisionDetectionType >( i );
                        }

                        if ( isSelected )
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                // TODO: Physics Material

                ImGui::DragFloat( "Mass", &rigidbodyComponent.Mass, 0.03f );
                ImGui::DragFloat( "Linear Drag", &rigidbodyComponent.LinearDrag, 0.01f );
                ImGui::DragFloat( "Angular Drag", &rigidbodyComponent.AngularDrag, 0.01f );
                ImGui::Checkbox( "Disable Gravity", &rigidbodyComponent.DisableGravity );
                ImGui::Checkbox( "Kinematic", &rigidbodyComponent.IsKinematic );

                ImGui::Separator();

                ImGui::Checkbox( "Lock Position X", &rigidbodyComponent.LockPositionX );
                ImGui::Checkbox( "Lock Position Y", &rigidbodyComponent.LockPositionY );
                ImGui::Checkbox( "Lock Position Z", &rigidbodyComponent.LockPositionZ );

                ImGui::Separator();

                ImGui::Checkbox( "Lock Rotation X", &rigidbodyComponent.LockRotationX );
                ImGui::Checkbox( "Lock Rotation Y", &rigidbodyComponent.LockRotationY );
                ImGui::Checkbox( "Lock Rotation Z", &rigidbodyComponent.LockRotationZ );
            } );

        DrawComponent< BoxColliderComponent >( "Box Collider",
            entity,
            []( auto &boxColliderComponent )
            {
                ImGui::DragFloat3( "Size", &boxColliderComponent.Size.x, 0.03f );
                ImGui::DragFloat3( "Offset", &boxColliderComponent.Offset.x, 0.03f );
                ImGui::Checkbox( "Trigger", &boxColliderComponent.IsTrigger );
                ImGui::Checkbox( "Show Collider Bounds", &boxColliderComponent.ShowColliderBounds );

                // TODO: Physics Material
            } );

        DrawComponent< SphereColliderComponent >( "Sphere Collider",
            entity,
            []( auto &sphereColliderComponent )
            {
                ImGui::DragFloat( "Radius", &sphereColliderComponent.Radius, 0.03f );
                ImGui::Checkbox( "Trigger", &sphereColliderComponent.IsTrigger );
                ImGui::Checkbox( "Show Collider Bounds", &sphereColliderComponent.ShowColliderBounds );
            } );

        DrawComponent< CapsuleColliderComponent >( "Capsule Collider",
            entity,
            []( auto &capsuleColliderComponent )
            {
                ImGui::DragFloat( "Radius", &capsuleColliderComponent.Radius, 0.03f );
                ImGui::DragFloat( "Height", &capsuleColliderComponent.Height, 0.03f );
                ImGui::Checkbox( "Trigger", &capsuleColliderComponent.IsTrigger );
                ImGui::Checkbox( "Show Collider Bounds", &capsuleColliderComponent.ShowColliderBounds );
            } );
    }

    template < typename ComponentType, typename UIFunction >
    void SceneHierarchyPanel::DrawComponent( const std::string &label,
        Entity entity,
        UIFunction uiFunction,
        bool isRemoveable )
    {
        if ( !entity.HasComponent< ComponentType >() )
            return;

        auto &component = entity.GetComponent< ComponentType >();
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                                 ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding |
                                                 ImGuiTreeNodeFlags_SpanAvailWidth;

        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4.f, 4.f } );

        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImGui::Separator();

        bool isOpen = ImGui::TreeNodeEx( ( void * )typeid( ComponentType ).hash_code(), treeNodeFlags, label.c_str() );
        ImGui::PopStyleVar();

        ImGui::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( ImGui::Button( "+", ImVec2{ lineHeight, lineHeight } ) )
        {
            ImGui::OpenPopup( "ComponentSettings" );
        }

        bool removeComponent = false;
        if ( ImGui::BeginPopup( "ComponentSettings" ) )
        {
            if ( ImGui::MenuItem( "Remove Component" ) )
                removeComponent = true;

            ImGui::EndPopup();
        }

        if ( isOpen )
        {
            uiFunction( component );
            ImGui::TreePop();
        }

        if ( isRemoveable && removeComponent )
            entity.RemoveComponent< ComponentType >();
    }

    void SceneHierarchyPanel::DrawMaterial( const Ref< graphic::Material > &pMaterial )
    {
        const Ref< graphic::Shader > &pShader = pMaterial->GetShader();
        ImGui::Text( "Material" );

        ImGui::Button( pShader->Name.c_str(), { 100.f, 0.0f } );
        if ( ImGui::BeginDragDropTarget() )
        {
            const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
            if ( pPayload )
            {
                const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                std::filesystem::path shaderPath = std::filesystem::path{ path };
                pMaterial->SetShader( graphic::RenderEngine::GetDevice()->CreateShader( shaderPath.string() ) );
            }

            ImGui::EndDragDropTarget();
        }

        const auto &floatValues{ pMaterial->GetFloatValues() };
        for ( const auto &pair : floatValues )
        {
            float value = pMaterial->GetFloatValue( pair.first );
            ImGui::DragFloat( pair.first.c_str(), &value, 0.03f );
            pMaterial->SetFloatValue( pair.first, value );
        }

        const auto &intValues{ pMaterial->GetIntValues() };
        for ( const auto &pair : intValues )
        {
            int value = pMaterial->GetIntValue( pair.first );
            ImGui::DragInt( pair.first.c_str(), &value, 0.03f );
            pMaterial->SetIntValue( pair.first, value );
        }

        const auto &boolValues{ pMaterial->GetBoolValues() };
        for ( const auto &pair : boolValues )
        {
            bool value = pMaterial->GetBoolValue( pair.first );
            ImGui::Checkbox( pair.first.c_str(), &value );
            pMaterial->SetBoolValue( pair.first, value );
        }

        const auto &float2Values{ pMaterial->GetFloat2Values() };
        for ( const auto &pair : float2Values )
        {
            DirectX::XMFLOAT2 value = pMaterial->GetFloat2Value( pair.first );
            ImGui::DragFloat2( pair.first.c_str(), &value.x, 0.03f );
            pMaterial->SetFloat2Value( pair.first, value );
        }

        const auto &float3Values{ pMaterial->GetFloat3Values() };
        for ( const auto &pair : float3Values )
        {
            DirectX::XMFLOAT3 value = pMaterial->GetFloat3Value( pair.first );
            ImGui::DragFloat3( pair.first.c_str(), &value.x, 0.03f );
            pMaterial->SetFloat3Value( pair.first, value );
        }

        const auto &texture2DValues{ pMaterial->GetTexture2DValues() };
        for ( const auto &pair : texture2DValues )
        {
            ImGui::Button( pair.first.c_str(), { 100.f, 0.0f } );
            if ( ImGui::BeginDragDropTarget() )
            {
                const ImGuiPayload *pPayload = ImGui::AcceptDragDropPayload( "ContentBrowserItem" );
                if ( pPayload )
                {
                    const wchar_t *path = static_cast< const wchar_t * >( pPayload->Data );
                    std::filesystem::path texturePath = std::filesystem::path{ path };
                    pMaterial->SetTexture2D(
                        pair.first, graphic::RenderEngine::GetDevice()->CreateTexture2D( texturePath.string() ) );
                }

                ImGui::EndDragDropTarget();
            }
        }
    }
}