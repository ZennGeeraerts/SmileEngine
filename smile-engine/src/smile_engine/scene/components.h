/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/core/uuid.h"
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "smile_engine/graphic/resource/index_buffer.h"
#include "smile_engine/graphic/shader/shader.h"

#include "smile_engine/scene/scene_camera.h"

#include "smile_engine/graphic/render_engine.h"
#include "smile_engine/graphic/mesh/model_loader.h"
#include "smile_engine/graphic/mesh/mesh_filter.h"
#include "smile_engine/graphic/mesh/skinned_mesh_filter.h"
#include "smile_engine/graphic/mesh/material.h"
#include "smile_engine/graphic/mesh/mesh_factory.h"

#include "smile_engine/physics/physics_material.h"

#include <DirectXMath.h>

namespace smile::scene
{
    struct IDComponent
    {
        IDComponent() = default;
        IDComponent( const IDComponent & ) = default;

        UUID ID;
    };

    struct TagComponent
    {
        TagComponent() = default;
        TagComponent( const TagComponent & ) = default;
        TagComponent( const std::string &tag ) : Tag{ tag }
        {
        }

        std::string Tag;
    };

    struct TransformComponent final
    {
        TransformComponent() = default;
        TransformComponent( const TransformComponent & ) = default;
        TransformComponent( const DirectX::XMFLOAT3 &translation,
            const DirectX::XMFLOAT3 &rotation,
            const DirectX::XMFLOAT3 &scale )
            : Translation{ translation }, Rotation{ rotation }, Scale{ scale }
        {
        }

        DirectX::XMFLOAT4X4 GetTransform() const
        {
            DirectX::XMMATRIX transformMat =
                DirectX::XMMatrixScaling( Scale.x, Scale.y, Scale.z ) *
                DirectX::XMMatrixRotationRollPitchYaw( Rotation.x, Rotation.y, Rotation.z ) *
                DirectX::XMMatrixTranslation( Translation.x, Translation.y, Translation.z );

            DirectX::XMFLOAT4X4 transform{};
            DirectX::XMStoreFloat4x4( &transform, transformMat );
            return transform;
        }

        DirectX::XMFLOAT3 GetForward()
        {
            DirectX::XMFLOAT3 forward{ 0, 0, 1 };
            RotateVector( forward );
            return forward;
        }

        DirectX::XMFLOAT3 GetRight()
        {
            DirectX::XMFLOAT3 right{ 1, 0, 0 };
            RotateVector( right );
            return right;
        }

        DirectX::XMFLOAT3 Translation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 Rotation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 Scale{ 1.f, 1.f, 1.f };

      private:
        void RotateVector( DirectX::XMFLOAT3 &v )
        {
            DirectX::XMVECTOR rotationVec =
                DirectX::XMQuaternionRotationRollPitchYaw( Rotation.x, Rotation.y, Rotation.z );
            auto rotationMat = DirectX::XMMatrixRotationQuaternion( rotationVec );

            DirectX::XMVECTOR vVec =
                DirectX::XMVector3TransformCoord( DirectX::XMVectorSet( v.x, v.y, v.z, 0 ), rotationMat );
            DirectX::XMStoreFloat3( &v, vVec );
        }
    };

    struct MeshRendererComponent final
    {
        MeshRendererComponent()
        {
            graphic::GraphicsDevice *pDevice = graphic::RenderEngine::GetDevice();
            auto pShader = pDevice->CreateShader( "assets/shaders/PBR.fx" );
            pMaterial = CreateRef< graphic::Material >( pShader );
        }

        MeshRendererComponent( const MeshRendererComponent & ) = default;

        MeshRendererComponent( const std::string &assetFile, Uint32 meshIndex, const Ref< graphic::Material > &pMaterial )
            : pMaterial{ pMaterial }, MeshIndex{ meshIndex }
        {
            pModel = graphic::ModelLoader::LoadModel( assetFile );
            Ref< graphic::MeshFilter > pMeshFilter = pModel->GetMeshFilter( meshIndex );
            pMesh = graphic::MeshFactory::CreateMesh( pMeshFilter, pMaterial->GetBufferLayout() );
        }

        Ref< graphic::Mesh > pMesh = nullptr;
        Ref< graphic::Material > pMaterial = nullptr;

         // For serialization
        Ref< graphic::Model > pModel = nullptr;
        Uint32 MeshIndex = 0;
    };

    struct SkinnedMeshRendererComponent final
    {
        SkinnedMeshRendererComponent()
        {
            graphic::GraphicsDevice *pDevice = graphic::RenderEngine::GetDevice();
            auto pShader = pDevice->CreateShader( "assets/shaders/PBR_Skinned.fx" );
            pMaterial = CreateRef< graphic::Material >( pShader );
        }

        SkinnedMeshRendererComponent( const SkinnedMeshRendererComponent & ) = default;

        SkinnedMeshRendererComponent( const std::string &assetFile, Uint32 meshIndex, const Ref< graphic::Material > &pMaterial )
            : pMaterial{ pMaterial }, MeshIndex{ meshIndex }
        {
            pModel = graphic::ModelLoader::LoadModel( assetFile );
            Ref< graphic::SkinnedMeshFilter > pSkinnedMeshFilter = pModel->GetSkinnedMeshFilter( meshIndex );
            pSkinnedMesh = graphic::MeshFactory::CreateSkinnedMesh( pSkinnedMeshFilter, pMaterial->GetBufferLayout() );
        }

        Ref< graphic::SkinnedMesh > pSkinnedMesh = nullptr;
        Ref< graphic::Material > pMaterial = nullptr;
        
        // For serialization
        Ref< graphic::Model > pModel = nullptr;
        Uint32 MeshIndex = 0;
    };

    struct AnimatorComponent final
    {
        AnimatorComponent() = default;
        AnimatorComponent( const AnimatorComponent & ) = default;

        std::vector< Ref< graphic::AnimationClip > > pAnimationClips;
        Uint32 CurrentClipIndex;

        // For serialization
        Ref< graphic::Model > pModel = nullptr;

        // For internal use
        std::vector< DirectX::XMFLOAT4X4 > Transforms;
        float TickCount = 0.f;
        bool IsPlaying = true;
        bool IsReversed = false;
    };

    struct CameraComponent final
    {
        CameraComponent() = default;
        CameraComponent( const CameraComponent & ) = default;

        SceneCamera Camera;
        bool IsPrimary = true;
        bool HasFixedAspectRatio = false;
    };

    struct ScriptComponent final
    {
        ScriptComponent() = default;
        ScriptComponent( const ScriptComponent & ) = default;

        std::string ClassName;
    };

    struct RigidbodyComponent final
    {
        enum class BodyType
        {
            Static = 0,
            Dynamic = 1
        };
        enum class CollisionDetectionType
        {
            Discrete = 0,
            Continuous = 1
        };

        RigidbodyComponent() = default;
        RigidbodyComponent( const RigidbodyComponent & ) = default;

        // Data
        BodyType Type;
        CollisionDetectionType CollisionDetection = CollisionDetectionType::Discrete;
        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;

        float Mass = 1.0f;
        float LinearDrag = 0.0f;
        float AngularDrag = 0.05f;

        bool DisableGravity = false;
        bool IsKinematic = false;

        bool LockPositionX = false;
        bool LockPositionY = false;
        bool LockPositionZ = false;
        bool LockRotationX = false;
        bool LockRotationY = false;
        bool LockRotationZ = false;
    };

    struct BoxColliderComponent final
    {
        BoxColliderComponent()
        {
            graphic::BufferLayout bufferLayout{ { graphic::ShaderDataType::Float3, "POSITION" } };
            pWireframeMesh = graphic::MeshFactory::CreateCube( bufferLayout );
        }

        BoxColliderComponent( const BoxColliderComponent & ) = default;

        DirectX::XMFLOAT3 Size = { 1.0f, 1.0f, 1.0f };
        DirectX::XMFLOAT3 Offset = { 0.0f, 0.0f, 0.0f };
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
        Ref< graphic::Mesh > pWireframeMesh = nullptr;
    };

    struct SphereColliderComponent final
    {
        SphereColliderComponent() = default;
        SphereColliderComponent( const SphereColliderComponent & ) = default;

        float Radius = 0.5f;
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
    };

    struct CapsuleColliderComponent final
    {
        CapsuleColliderComponent() = default;
        CapsuleColliderComponent( const CapsuleColliderComponent & ) = default;

        float Radius = 0.5f;
        float Height = 1.0f;
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
    };

    struct SpriteRendererComponent final
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent( const SpriteRendererComponent & ) = default;
        SpriteRendererComponent( const DirectX::XMFLOAT4 &color ) : Color{ color }
        {
        }

        DirectX::XMFLOAT4 Color = { 1.f, 1.f, 1.f, 1.f };
        Ref< graphic::Texture > pTexture = nullptr;
    };

    template < typename... Component >
    struct ComponentGroup
    {
    };

    using AllComponents = ComponentGroup< TransformComponent,
        MeshRendererComponent,
        SkinnedMeshRendererComponent,
        AnimatorComponent,
        CameraComponent,
        ScriptComponent,
        RigidbodyComponent,
        BoxColliderComponent,
        SphereColliderComponent,
        CapsuleColliderComponent,
        SpriteRendererComponent >;
}