#pragma once
#include "SmileEngine/Core/UUID.h"
#include "SmileEngine/Renderer/VertexBuffer.h"
#include "SmileEngine/Renderer/IndexBuffer.h"
#include "SmileEngine/Renderer/Shader.h"

#include "SmileEngine/Scene/SceneCamera.h"

#include "SmileEngine/Renderer/Mesh/MeshLoader.h"
#include "SmileEngine/Renderer/Mesh/StaticMeshFilter.h"
#include "SmileEngine/Renderer/Mesh/SkinnedMeshFilter.h"
#include "SmileEngine/Renderer/Mesh/Material.h"
#include "SmileEngine/Renderer/Animation/MeshAnimator.h"
#include "SmileEngine/Renderer/Mesh/MeshFactory.h"

#include "SmileEngine/Physics/PhysicsMaterial.h"

#include <DirectXMath.h>

namespace smile
{
    struct IDComponent
    {
        IDComponent() = default;
        IDComponent( const IDComponent & ) = default;

        UUID m_ID;
    };

    struct TagComponent
    {
        TagComponent() = default;
        TagComponent( const TagComponent & ) = default;
        TagComponent( const std::string &tag ) : m_Tag{ tag }
        {
        }

        std::string m_Tag;
    };

    struct TransformComponent final
    {
        TransformComponent() = default;
        TransformComponent( const TransformComponent & ) = default;
        TransformComponent( const DirectX::XMFLOAT3 &translation,
            const DirectX::XMFLOAT3 &rotation,
            const DirectX::XMFLOAT3 &scale )
            : m_Translation{ translation }, m_Rotation{ rotation }, m_Scale{ scale }
        {
        }

        DirectX::XMFLOAT4X4 GetTransform() const
        {
            DirectX::XMMATRIX transformMat =
                DirectX::XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z ) *
                DirectX::XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z ) *
                DirectX::XMMatrixTranslation( m_Translation.x, m_Translation.y, m_Translation.z );

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

        DirectX::XMFLOAT3 m_Translation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 m_Rotation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 m_Scale{ 1.f, 1.f, 1.f };

      private:
        void RotateVector( DirectX::XMFLOAT3 &v )
        {
            DirectX::XMVECTOR rotationVec =
                DirectX::XMQuaternionRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
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
        MeshRendererComponent( const VertexBufferDescriptor &vertexBufferData,
            const IndexBufferDescriptor &indexBufferData,
            const std::string &shaderFilePath )
        {
            m_pVertexBuffer.reset( VertexBuffer::Create( vertexBufferData ) );
            m_pIndexBuffer.reset( IndexBuffer::Create( indexBufferData ) );
            m_pShader = Shader::Create( shaderFilePath );
        }

        Ref< VertexBuffer > m_pVertexBuffer = nullptr;
        Ref< IndexBuffer > m_pIndexBuffer = nullptr;
        Ref< Shader > m_pShader = nullptr;
    };

    struct StaticMeshComponent final
    {
        StaticMeshComponent()
        {
            m_pMaterials.push_back( CreateRef< Material >() );
        }

        StaticMeshComponent( const StaticMeshComponent & ) = default;

        // For now, only support 1 material
        StaticMeshComponent( const std::string &assetFile, const Ref< Material > &pMaterial )
        {
            m_pMaterials.push_back( pMaterial );

            m_pMeshes = MeshLoader::LoadStaticMesh( assetFile );
            const auto &bufferLayout = m_pMaterials[0]->GetBufferLayout();
            for ( const auto &pMesh : m_pMeshes )
            {
                pMesh->Create( bufferLayout );
            }
        }

        std::vector< Ref< StaticMeshFilter > > m_pMeshes = {};
        std::vector< Ref< Material > > m_pMaterials = {};
    };

    struct SkinnedMeshComponent final
    {
        SkinnedMeshComponent()
        {
            m_pMaterials.push_back( CreateRef< Material >( true ) );
        }

        SkinnedMeshComponent( const SkinnedMeshComponent & ) = default;

        // For now, only support 1 material
        SkinnedMeshComponent( const std::string &assetFile, const Ref< Material > &pMaterial )
        {
            m_pMaterials.push_back( pMaterial );

            m_pMeshes = MeshLoader::LoadSkinnedMesh( assetFile );
            const auto &bufferLayout = m_pMaterials[0]->GetBufferLayout();
            for ( const auto &pMesh : m_pMeshes )
            {
                pMesh->Create( bufferLayout );

                if ( pMesh->HasAnimations() )
                {
                    MeshAnimator animator{ pMesh };
                    m_Animators.push_back( animator );
                }
            }
        }

        std::vector< Ref< SkinnedMeshFilter > > m_pMeshes = {};
        std::vector< Ref< Material > > m_pMaterials = {};
        std::vector< MeshAnimator > m_Animators = {};
    };

    struct CameraComponent final
    {
        CameraComponent() = default;
        CameraComponent( const CameraComponent & ) = default;

        SceneCamera m_Camera;
        bool m_bPrimary = true;
        bool m_bFixedAspectRatio = false;
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
        BodyType m_BodyType;
        CollisionDetectionType m_CollisionDetectionType = CollisionDetectionType::Discrete;
        Ref< PhysicsMaterial > m_pPhysicsMaterial = nullptr;

        float m_Mass = 1.0f;
        float m_LinearDrag = 0.0f;
        float m_AngularDrag = 0.05f;

        bool m_bDisableGravity = false;
        bool m_bKinematic = false;

        bool m_bLockPositionX = false;
        bool m_bLockPositionY = false;
        bool m_bLockPositionZ = false;
        bool m_bLockRotationX = false;
        bool m_bLockRotationY = false;
        bool m_bLockRotationZ = false;
    };

    struct BoxColliderComponent final
    {
        BoxColliderComponent()
        {
            BufferLayout bufferLayout{ { ShaderDataType::Float3, "Position" } };
            m_pWireframeMesh = MeshFactory::CreateCube( bufferLayout );
            m_pWireframeMesh->Create( bufferLayout );
        }

        BoxColliderComponent( const BoxColliderComponent & ) = default;

        DirectX::XMFLOAT3 m_Size = { 1.0f, 1.0f, 1.0f };
        DirectX::XMFLOAT3 m_Offset = { 0.0f, 0.0f, 0.0f };
        bool m_bTrigger = false;
        bool m_bShowColliderBounds = true;

        Ref< PhysicsMaterial > m_pPhysicsMaterial = nullptr;
        Ref< StaticMeshFilter > m_pWireframeMesh = nullptr;
    };

    struct SphereColliderComponent final
    {
        SphereColliderComponent() = default;
        SphereColliderComponent( const SphereColliderComponent & ) = default;

        float m_Radius = 0.5f;
        bool m_bTrigger = false;
        bool m_bShowColliderBounds = true;

        Ref< PhysicsMaterial > m_pPhysicsMaterial = nullptr;
    };

    struct CapsuleColliderComponent final
    {
        CapsuleColliderComponent() = default;
        CapsuleColliderComponent( const CapsuleColliderComponent & ) = default;

        float m_Radius = 0.5f;
        float m_Height = 1.0f;
        bool m_bTrigger = false;
        bool m_bShowColliderBounds = true;

        Ref< PhysicsMaterial > m_pPhysicsMaterial = nullptr;
    };
}