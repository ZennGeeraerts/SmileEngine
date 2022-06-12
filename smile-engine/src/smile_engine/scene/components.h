#pragma once
#include "smile_engine/core/uuid.h"
#include "smile_engine/renderer/resource/vertex_buffer.h"
#include "smile_engine/renderer/resource/index_buffer.h"
#include "smile_engine/renderer/shader/shader.h"

#include "smile_engine/scene/scene_camera.h"

#include "smile_engine/renderer/mesh/mesh_loader.h"
#include "smile_engine/renderer/mesh/static_mesh_filter.h"
#include "smile_engine/renderer/mesh/skinned_mesh_filter.h"
#include "smile_engine/renderer/mesh/material.h"
#include "smile_engine/renderer/animation/mesh_animator.h"
#include "smile_engine/renderer/mesh/mesh_factory.h"

#include "smile_engine/physics/physics_material.h"

#include <DirectXMath.h>

namespace smile::scene
{
    struct IDComponent
    {
        IDComponent() = default;
        IDComponent( const IDComponent & ) = default;

        UUID id;
    };

    struct TagComponent
    {
        TagComponent() = default;
        TagComponent( const TagComponent & ) = default;
        TagComponent( const std::string &tag ) : tag{ tag }
        {
        }

        std::string tag;
    };

    struct TransformComponent final
    {
        TransformComponent() = default;
        TransformComponent( const TransformComponent & ) = default;
        TransformComponent( const DirectX::XMFLOAT3 &translation,
            const DirectX::XMFLOAT3 &rotation,
            const DirectX::XMFLOAT3 &scale )
            : translation{ translation }, rotation{ rotation }, scale{ scale }
        {
        }

        DirectX::XMFLOAT4X4 getTransform() const
        {
            DirectX::XMMATRIX transform_mat =
                DirectX::XMMatrixScaling( scale.x, scale.y, scale.z ) *
                DirectX::XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *
                DirectX::XMMatrixTranslation( translation.x, translation.y, translation.z );

            DirectX::XMFLOAT4X4 transform{};
            DirectX::XMStoreFloat4x4( &transform, transform_mat );
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

        DirectX::XMFLOAT3 translation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 rotation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 scale{ 1.f, 1.f, 1.f };

      private:
        void rotateVector( DirectX::XMFLOAT3 &v )
        {
            DirectX::XMVECTOR rotation_vec =
                DirectX::XMQuaternionRotationRollPitchYaw( rotation.x, rotation.y, rotation.z );
            auto rotation_mat = DirectX::XMMatrixRotationQuaternion( rotation_vec );

            DirectX::XMVECTOR v_vec =
                DirectX::XMVector3TransformCoord( DirectX::XMVectorSet( v.x, v.y, v.z, 0 ), rotation_mat );
            DirectX::XMStoreFloat3( &v, v_vec );
        }
    };

    struct MeshRendererComponent final
    {
        MeshRendererComponent() = default;
        MeshRendererComponent( const MeshRendererComponent & ) = default;
        MeshRendererComponent( const renderer::VertexBufferDescriptor &vertex_buffer_desc,
            const renderer::IndexBufferDescriptor &index_buffer_desc,
            const std::string &shader_file_path )
        {
            vertexBuffer.reset( renderer::VertexBuffer::create( vertex_buffer_desc ) );
            indexBuffer.reset( renderer::IndexBuffer::create( index_buffer_desc ) );
            shader = renderer::Shader::create( shader_file_path );
        }

        Ref< renderer::VertexBuffer > vertexBuffer = nullptr;
        Ref< renderer::IndexBuffer > indexBuffer = nullptr;
        Ref< renderer::Shader > shader = nullptr;
    };

    struct StaticMeshComponent final
    {
        StaticMeshComponent()
        {
            auto shader = renderer::Shader::create( "assets/shaders/PBR.fx" );
            materials.push_back( createRef< renderer::Material >( shader ) );
        }

        StaticMeshComponent( const StaticMeshComponent & ) = default;

        // For now, only support 1 material
        StaticMeshComponent( const std::string &asset_file, const Ref< renderer::Material > &material )
        {
            materials.push_back( material );

            meshes = renderer::MeshLoader::loadStaticMesh( asset_file );
            const auto &buffer_layout = materials[0]->getBufferLayout();
            for ( const auto &mesh : meshes )
            {
                mesh->create( buffer_layout );
            }
        }

        std::vector< Ref< renderer::StaticMeshFilter > > meshes = {};
        std::vector< Ref< renderer::Material > > materials = {};
    };

    struct SkinnedMeshComponent final
    {
        SkinnedMeshComponent()
        {
            auto pShader = renderer::Shader::create( "assets/shaders/PBR_Skinned.fx" );
            materials.push_back( createRef< renderer::Material >( pShader ) );
        }

        SkinnedMeshComponent( const SkinnedMeshComponent & ) = default;

        // For now, only support 1 material
        SkinnedMeshComponent( const std::string &asset_file, const Ref< renderer::Material > &material )
        {
            materials.push_back( material );

            meshes = renderer::MeshLoader::loadSkinnedMesh( asset_file );
            const auto &buffer_layout = materials[0]->getBufferLayout();
            for ( const auto &mesh : meshes )
            {
                mesh->create( buffer_layout );

                if ( mesh->hasAnimations() )
                {
                    renderer::MeshAnimator animator{ mesh };
                    animators.push_back( animator );
                }
            }
        }

        std::vector< Ref< renderer::SkinnedMeshFilter > > meshes = {};
        std::vector< Ref< renderer::Material > > materials = {};
        std::vector< renderer::MeshAnimator > animators = {};
    };

    struct CameraComponent final
    {
        CameraComponent() = default;
        CameraComponent( const CameraComponent & ) = default;

        SceneCamera camera;
        bool primary = true;
        bool fixedAspectRatio = false;
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
        BodyType bodyType;
        CollisionDetectionType collisionDetectionType = CollisionDetectionType::Discrete;
        Ref< physics::PhysicsMaterial > physicsMaterial = nullptr;

        float mass = 1.0f;
        float linearDrag = 0.0f;
        float angularDrag = 0.05f;

        bool disableGravity = false;
        bool kinematic = false;

        bool lockPositionX = false;
        bool lockPositionY = false;
        bool lockPositionZ = false;
        bool lockRotationX = false;
        bool lockRotationY = false;
        bool lockRotationZ = false;
    };

    struct BoxColliderComponent final
    {
        BoxColliderComponent()
        {
            renderer::BufferLayout buffer_layout{ { renderer::ShaderDataType::Float3, "POSITION" } };
            wireframeMesh = renderer::MeshFactory::createCube( buffer_layout );
            wireframeMesh->create( buffer_layout );
        }

        BoxColliderComponent( const BoxColliderComponent & ) = default;

        DirectX::XMFLOAT3 size = { 1.0f, 1.0f, 1.0f };
        DirectX::XMFLOAT3 offset = { 0.0f, 0.0f, 0.0f };
        bool trigger = false;
        bool showColliderBounds = true;

        Ref< physics::PhysicsMaterial > physicsMaterial = nullptr;
        Ref< renderer::StaticMeshFilter > wireframeMesh = nullptr;
    };

    struct SphereColliderComponent final
    {
        SphereColliderComponent() = default;
        SphereColliderComponent( const SphereColliderComponent & ) = default;

        float radius = 0.5f;
        bool trigger = false;
        bool showColliderBounds = true;

        Ref< physics::PhysicsMaterial > physicsMaterial = nullptr;
    };

    struct CapsuleColliderComponent final
    {
        CapsuleColliderComponent() = default;
        CapsuleColliderComponent( const CapsuleColliderComponent & ) = default;

        float radius = 0.5f;
        float height = 1.0f;
        bool trigger = false;
        bool showColliderBounds = true;

        Ref< physics::PhysicsMaterial > physicsMaterial = nullptr;
    };
}