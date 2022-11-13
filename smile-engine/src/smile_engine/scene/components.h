#pragma once
#include "smile_engine/core/uuid.h"
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "smile_engine/graphic/resource/index_buffer.h"
#include "smile_engine/graphic/shader/shader.h"

#include "smile_engine/scene/scene_camera.h"

#include "smile_engine/graphic/mesh/mesh_loader.h"
#include "smile_engine/graphic/mesh/static_mesh_filter.h"
#include "smile_engine/graphic/mesh/skinned_mesh_filter.h"
#include "smile_engine/graphic/mesh/material.h"
#include "smile_engine/graphic/animation/mesh_animator.h"
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

        DirectX::XMFLOAT3 getForward()
        {
            DirectX::XMFLOAT3 forward{ 0, 0, 1 };
            rotateVector( forward );
            return forward;
        }

        DirectX::XMFLOAT3 getRight()
        {
            DirectX::XMFLOAT3 right{ 1, 0, 0 };
            rotateVector( right );
            return right;
        }

        DirectX::XMFLOAT3 Translation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 Rotation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 Scale{ 1.f, 1.f, 1.f };

      private:
        void rotateVector( DirectX::XMFLOAT3 &v )
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
        MeshRendererComponent() = default;
        MeshRendererComponent( const MeshRendererComponent & ) = default;
        MeshRendererComponent( const graphic::VertexBufferDescriptor &vertexBufferDesc,
            const graphic::IndexBufferDescriptor &indexBufferDesc,
            const std::string &shaderFilePath )
        {
            pVertexBuffer.reset( graphic::VertexBuffer::Create( vertexBufferDesc ) );
            pIndexBuffer.reset( graphic::IndexBuffer::Create( indexBufferDesc ) );
            pShader = graphic::Shader::Create( shaderFilePath );
        }

        Ref< graphic::VertexBuffer > pVertexBuffer = nullptr;
        Ref< graphic::IndexBuffer > pIndexBuffer = nullptr;
        Ref< graphic::Shader > pShader = nullptr;
    };

    struct StaticMeshComponent final
    {
        StaticMeshComponent()
        {
            auto pShader = graphic::Shader::Create( "assets/shaders/PBR.fx" );
            pMaterials.push_back( CreateRef< graphic::Material >( pShader ) );
        }

        StaticMeshComponent( const StaticMeshComponent & ) = default;

        // For now, only support 1 material
        StaticMeshComponent( const std::string &assetFile, const Ref< graphic::Material > &pMaterial )
        {
            pMaterials.push_back( pMaterial );

            pMeshes = graphic::MeshLoader::LoadStaticMesh( assetFile );
            const auto &bufferLayout = pMaterials[0]->GetBufferLayout();
            for ( const auto &pMesh : pMeshes )
            {
                pMesh->Create( bufferLayout );
            }
        }

        std::vector< Ref< graphic::StaticMeshFilter > > pMeshes = {};
        std::vector< Ref< graphic::Material > > pMaterials = {};
    };

    struct SkinnedMeshComponent final
    {
        SkinnedMeshComponent()
        {
            auto pShader = graphic::Shader::Create( "assets/shaders/PBR_Skinned.fx" );
            pMaterials.push_back( CreateRef< graphic::Material >( pShader ) );
        }

        SkinnedMeshComponent( const SkinnedMeshComponent & ) = default;

        // For now, only support 1 material
        SkinnedMeshComponent( const std::string &assetFile, const Ref< graphic::Material > &pMaterial )
        {
            pMaterials.push_back( pMaterial );

            pMeshes = graphic::MeshLoader::LoadSkinnedMesh( assetFile );
            const auto &bufferLayout = pMaterials[0]->GetBufferLayout();
            for ( const auto &pMesh : pMeshes )
            {
                pMesh->Create( bufferLayout );

                if ( pMesh->HasAnimations() )
                {
                    graphic::MeshAnimator animator{ pMesh };
                    Animators.push_back( animator );
                }
            }
        }

        std::vector< Ref< graphic::SkinnedMeshFilter > > pMeshes = {};
        std::vector< Ref< graphic::Material > > pMaterials = {};
        std::vector< graphic::MeshAnimator > Animators = {};
    };

    struct CameraComponent final
    {
        CameraComponent() = default;
        CameraComponent( const CameraComponent & ) = default;

        SceneCamera Camera;
        bool IsPrimary = true;
        bool HasFixedAspectRatio = false;
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
            pWireframeMesh->Create( bufferLayout );
        }

        BoxColliderComponent( const BoxColliderComponent & ) = default;

        DirectX::XMFLOAT3 Size = { 1.0f, 1.0f, 1.0f };
        DirectX::XMFLOAT3 Offset = { 0.0f, 0.0f, 0.0f };
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
        Ref< graphic::StaticMeshFilter > pWireframeMesh = nullptr;
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
}