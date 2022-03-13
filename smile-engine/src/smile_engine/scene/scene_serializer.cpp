#include "smpch.h"
#include "scene_serializer.h"

#include "smile_engine/core/logger.h"
#include "entity.h"
#include "components.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML
{
    template <>
    struct convert< DirectX::XMFLOAT3 >
    {
        static Node encode( const DirectX::XMFLOAT3 &v )
        {
            Node node{};
            node.push_back( v.x );
            node.push_back( v.y );
            node.push_back( v.z );
            return node;
        }

        static bool decode( const Node &node, DirectX::XMFLOAT3 &v )
        {
            if ( !node.IsSequence() || node.size() != 3 )
                return false;

            v.x = node[0].as< float >();
            v.y = node[1].as< float >();
            v.z = node[2].as< float >();
            return true;
        }
    };

    template <>
    struct convert< DirectX::XMFLOAT4 >
    {
        static Node encode( const DirectX::XMFLOAT4 &v )
        {
            Node node{};
            node.push_back( v.x );
            node.push_back( v.y );
            node.push_back( v.z );
            node.push_back( v.w );
            return node;
        }

        static bool decode( const Node &node, DirectX::XMFLOAT4 &v )
        {
            if ( !node.IsSequence() || node.size() != 4 )
                return false;

            v.x = node[0].as< float >();
            v.y = node[1].as< float >();
            v.z = node[2].as< float >();
            v.w = node[3].as< float >();
            return true;
        }
    };
}

namespace smile
{
    YAML::Emitter &operator<<( YAML::Emitter &output, const DirectX::XMFLOAT3 &v )
    {
        output << YAML::Flow;
        output << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return output;
    }

    YAML::Emitter &operator<<( YAML::Emitter &output, const DirectX::XMFLOAT4 &v )
    {
        output << YAML::Flow;
        output << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return output;
    }

    SceneSerializer::SceneSerializer( const Ref< Scene > &pScene ) : m_pScene{ pScene }
    {
    }

    void SceneSerializer::Serialize( const std::string &filePath )
    {
        YAML::Emitter output{};
        output << YAML::BeginMap;
        output << YAML::Key << "Scene" << YAML::Value << "Untitled";
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_pScene->m_Registry.each(
            [&]( auto entityID )
            {
                Entity entity{ entityID, m_pScene.get() };
                if ( !entity )
                    return;
                SerializeEntity( output, entity );
            } );

        output << YAML::EndSeq;
        output << YAML::EndMap;

        std::ofstream fileOutput{ filePath };
        fileOutput << output.c_str();
    }

    static void SerializeEntity( YAML::Emitter &output, Entity entity )
    {
        SM_ASSERT( entity.HasComponent< IDComponent >(),
            "SceneSerializer::SerializeScene > Entity does not have an IDComponent" );

        output << YAML::BeginMap;
        output << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        if ( entity.HasComponent< TagComponent >() )
        {
            output << YAML::Key << "TagComponent";
            output << YAML::BeginMap;

            auto &tag = entity.GetComponent< TagComponent >().m_Tag;
            output << YAML::Key << "Tag" << YAML::Value << tag;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< TransformComponent >() )
        {
            output << YAML::Key << "TransformComponent";
            output << YAML::BeginMap;

            auto &transformComponent = entity.GetComponent< TransformComponent >();
            output << YAML::Key << "Translation" << YAML::Value << transformComponent.m_Translation;
            output << YAML::Key << "Rotation" << YAML::Value << transformComponent.m_Rotation;
            output << YAML::Key << "Scale" << YAML::Value << transformComponent.m_Scale;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< CameraComponent >() )
        {
            output << YAML::Key << "CameraComponent";
            output << YAML::BeginMap;

            auto &cameraComponent = entity.GetComponent< CameraComponent >();
            auto &camera = cameraComponent.m_Camera;
            output << YAML::Key << "Camera" << YAML::Value;
            output << YAML::BeginMap;

            output << YAML::Key << "ProjectionType" << YAML::Value
                   << static_cast< uint32_t >( camera.GetProjectionType() );
            output << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
            output << YAML::Key << "PerspectiveNearPlane" << YAML::Value << camera.GetPerspectiveNearPlane();
            output << YAML::Key << "PerspectiveFarPlane" << YAML::Value << camera.GetPerspectiveFarPlane();
            output << YAML::Key << "Size" << YAML::Value << camera.GetSize();
            output << YAML::Key << "OrthographicNearPlane" << camera.GetOrthographicNearPlane();
            output << YAML::Key << "OrthographicFarPlane" << camera.GetOrthographicFarPlane();

            output << YAML::EndMap;

            output << YAML::Key << "bPrimary" << YAML::Value << cameraComponent.m_bPrimary;
            output << YAML::Key << "bFixedAspectRatio" << YAML::Value << cameraComponent.m_bFixedAspectRatio;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< StaticMeshComponent >() )
        {
            output << YAML::Key << "SkinnedMeshComponent";
            output << YAML::BeginMap;

            auto &meshComponent = entity.GetComponent< StaticMeshComponent >();
            output << YAML::Key << "Mesh" << YAML::Value
                   << ( ( meshComponent.m_pMeshes.size() > 0 ) ? meshComponent.m_pMeshes[0]->GetFilePath() : "" );

            output << YAML::Key << "Material";
            output << YAML::BeginMap;

            auto &pMaterial = meshComponent.m_pMaterials[0];
            output << YAML::Key << "AlbedoMap" << YAML::Value
                   << ( pMaterial->GetAlbedoMap() ? pMaterial->GetAlbedoMap()->GetFilePath() : "" );
            output << YAML::Key << "AlbedoColor" << YAML::Value << pMaterial->GetAlbedoColor();

            output << YAML::Key << "MetalnessMap" << YAML::Value
                   << ( pMaterial->GetMetalnessMap() ? pMaterial->GetMetalnessMap()->GetFilePath() : "" );
            output << YAML::Key << "Metalness" << YAML::Value << pMaterial->GetMetalness();

            output << YAML::Key << "RoughnessMap" << YAML::Value
                   << ( pMaterial->GetRoughnessMap() ? pMaterial->GetRoughnessMap()->GetFilePath() : "" );
            output << YAML::Key << "Roughness" << YAML::Value << pMaterial->GetRoughness();

            output << YAML::Key << "NormalMap" << YAML::Value
                   << ( pMaterial->GetNormalMap() ? pMaterial->GetNormalMap()->GetFilePath() : "" );
            output << YAML::Key << "AOMap" << YAML::Value
                   << ( pMaterial->GetAOMap() ? pMaterial->GetAOMap()->GetFilePath() : "" );

            output << YAML::EndMap;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< SkinnedMeshComponent >() )
        {
            output << YAML::Key << "SkinnedMeshComponent";
            output << YAML::BeginMap;

            auto &meshComponent = entity.GetComponent< SkinnedMeshComponent >();
            output << YAML::Key << "Mesh" << YAML::Value
                   << ( ( meshComponent.m_pMeshes.size() > 0 ) ? meshComponent.m_pMeshes[0]->GetFilePath() : "" );

            output << YAML::Key << "Material";
            output << YAML::BeginMap;

            auto &pMaterial = meshComponent.m_pMaterials[0];
            output << YAML::Key << "AlbedoMap" << YAML::Value
                   << ( pMaterial->GetAlbedoMap() ? pMaterial->GetAlbedoMap()->GetFilePath() : "" );
            output << YAML::Key << "AlbedoColor" << YAML::Value << pMaterial->GetAlbedoColor();

            output << YAML::Key << "MetalnessMap" << YAML::Value
                   << ( pMaterial->GetMetalnessMap() ? pMaterial->GetMetalnessMap()->GetFilePath() : "" );
            output << YAML::Key << "Metalness" << YAML::Value << pMaterial->GetMetalness();

            output << YAML::Key << "RoughnessMap" << YAML::Value
                   << ( pMaterial->GetRoughnessMap() ? pMaterial->GetRoughnessMap()->GetFilePath() : "" );
            output << YAML::Key << "Roughness" << YAML::Value << pMaterial->GetRoughness();

            output << YAML::Key << "NormalMap" << YAML::Value
                   << ( pMaterial->GetNormalMap() ? pMaterial->GetNormalMap()->GetFilePath() : "" );
            output << YAML::Key << "AOMap" << YAML::Value
                   << ( pMaterial->GetAOMap() ? pMaterial->GetAOMap()->GetFilePath() : "" );

            output << YAML::EndMap;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< RigidbodyComponent >() )
        {
            output << YAML::Key << "RigidbodyComponent";
            output << YAML::BeginMap;

            auto &rigidbodyComponent = entity.GetComponent< RigidbodyComponent >();
            output << YAML::Key << "BodyType" << YAML::Value << static_cast< uint32_t >( rigidbodyComponent.m_BodyType );
            output << YAML::Key << "CollisionDetectionType" << YAML::Value
                   << static_cast< uint32_t >( rigidbodyComponent.m_CollisionDetectionType );

            output << YAML::Key << "PhysicsMaterial";
            output << YAML::BeginMap;

            /*auto& pPhysicsMaterial = rigidbodyComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;

            output << YAML::Key << "Mass" << YAML::Value << rigidbodyComponent.m_Mass;
            output << YAML::Key << "LinearDrag" << YAML::Value << rigidbodyComponent.m_LinearDrag;
            output << YAML::Key << "AngularDrag" << YAML::Value << rigidbodyComponent.m_AngularDrag;
            output << YAML::Key << "bDisableGravity" << YAML::Value << rigidbodyComponent.m_bDisableGravity;
            output << YAML::Key << "bKinematic" << YAML::Value << rigidbodyComponent.m_bKinematic;

            output << YAML::Key << "bLockPositionX" << YAML::Value << rigidbodyComponent.m_bLockPositionX;
            output << YAML::Key << "bLockPositionY" << YAML::Value << rigidbodyComponent.m_bLockPositionY;
            output << YAML::Key << "bLockPositionZ" << YAML::Value << rigidbodyComponent.m_bLockPositionZ;

            output << YAML::Key << "bLockRotationX" << YAML::Value << rigidbodyComponent.m_bLockRotationX;
            output << YAML::Key << "bLockRotationY" << YAML::Value << rigidbodyComponent.m_bLockRotationY;
            output << YAML::Key << "bLockRotationZ" << YAML::Value << rigidbodyComponent.m_bLockRotationZ;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< BoxColliderComponent >() )
        {
            output << YAML::Key << "BoxColliderComponent";
            output << YAML::BeginMap;

            auto &boxColliderComponent = entity.GetComponent< BoxColliderComponent >();
            output << YAML::Key << "Size" << YAML::Value << boxColliderComponent.m_Size;
            output << YAML::Key << "Offset" << YAML::Value << boxColliderComponent.m_Offset;
            output << YAML::Key << "bTrigger" << YAML::Value << boxColliderComponent.m_bTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << boxColliderComponent.m_bShowColliderBounds;

            /*auto& pPhysicsMaterial = boxColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< SphereColliderComponent >() )
        {
            output << YAML::Key << "SphereColliderComponent";
            output << YAML::BeginMap;

            auto &sphereColliderComponent = entity.GetComponent< SphereColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent.m_Radius;
            output << YAML::Key << "bTrigger" << YAML::Value << sphereColliderComponent.m_bTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << sphereColliderComponent.m_bShowColliderBounds;

            /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< CapsuleColliderComponent >() )
        {
            output << YAML::Key << "CapsuleColliderComponent";
            output << YAML::BeginMap;

            auto &capsuleColliderComponent = entity.GetComponent< CapsuleColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent.m_Radius;
            output << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent.m_Height;
            output << YAML::Key << "bTrigger" << YAML::Value << capsuleColliderComponent.m_bTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << capsuleColliderComponent.m_bShowColliderBounds;

            output << YAML::EndMap;
        }

        output << YAML::EndMap;
    }

    void SceneSerializer::SerializeRuntime( const std::string &filePath )
    {
        SM_ASSERT( false, "SceneSerializer::SerializeRuntime > Not implemented" );
    }

    bool SceneSerializer::Deserialize( const std::string &filePath )
    {
        std::ifstream fileInput{ filePath };
        std::stringstream strStream{};
        strStream << fileInput.rdbuf();

        YAML::Node data = YAML::Load( strStream.str() );
        if ( !data["Scene"] )
            return false;

        std::string sceneName = data["Scene"].as< std::string >();
        SM_LOG_TRACE( "Deserializing scene '%s'", sceneName.c_str() );

        auto entities = data["Entities"];
        if ( entities )
        {
            for ( auto entity : entities )
            {
                uint64_t uuid = entity["Entity"].as< uint64_t >();

                std::string name{};
                auto tagComponent = entity["TagComponent"];
                if ( tagComponent )
                    name = tagComponent["Tag"].as< std::string >();

                SM_LOG_TRACE( "Deserialized entity with ID: %llu, name: %s", uuid, name.c_str() );

                Entity deserializedEntity = m_pScene->CreateEntity( uuid, name );

                auto transformComponent = entity["TransformComponent"];
                if ( transformComponent )
                {
                    auto &tc = deserializedEntity.GetComponent< TransformComponent >();
                    tc.m_Translation = transformComponent["Translation"].as< DirectX::XMFLOAT3 >();
                    tc.m_Rotation = transformComponent["Rotation"].as< DirectX::XMFLOAT3 >();
                    tc.m_Scale = transformComponent["Scale"].as< DirectX::XMFLOAT3 >();
                }

                auto cameraComponent = entity["CameraComponent"];
                if ( cameraComponent )
                {
                    auto &cc = deserializedEntity.AddComponent< CameraComponent >();

                    auto cameraProps = cameraComponent["Camera"];
                    cc.m_Camera.SetProjectionType(
                        static_cast< SceneCamera::ProjectionType >( cameraProps["ProjectionType"].as< int >() ) );

                    cc.m_Camera.SetFOV( cameraProps["FOV"].as< float >() );
                    cc.m_Camera.SetPerspectiveNearPlane( cameraProps["PerspectiveNearPlane"].as< float >() );
                    cc.m_Camera.SetPerspectiveFarPlane( cameraProps["PerspectiveFarPlane"].as< float >() );

                    cc.m_Camera.SetSize( cameraProps["Size"].as< float >() );
                    cc.m_Camera.SetOrthographicNearPlane( cameraProps["OrthographicNearPlane"].as< float >() );
                    cc.m_Camera.SetOrthographicFarPlane( cameraProps["OrthographicFarPlane"].as< float >() );

                    cc.m_bPrimary = cameraComponent["bPrimary"].as< bool >();
                    cc.m_bFixedAspectRatio = cameraComponent["bFixedAspectRatio"].as< bool >();
                }

                auto staticMeshComponent = entity["StaticMeshComponent"];
                if ( staticMeshComponent )
                {
                    auto &smc = deserializedEntity.AddComponent< StaticMeshComponent >();

                    const auto &meshPath = staticMeshComponent["Mesh"].as< std::string >();
                    if ( !meshPath.empty() )
                    {
                        smc.m_pMeshes = MeshLoader::LoadStaticMesh( meshPath );
                        const auto &bufferLayout = smc.m_pMaterials[0]->GetBufferLayout();
                        for ( const auto &pMesh : smc.m_pMeshes )
                        {
                            pMesh->Create( bufferLayout );
                        }
                    }

                    auto material = staticMeshComponent["Material"];

                    const auto &albedoMap = material["AlbedoMap"].as< std::string >();
                    if ( !albedoMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetAlbedo( Texture2D::Create( albedoMap ) );
                        smc.m_pMaterials[0]->SetUseAlbedoMap( true );
                    }
                    smc.m_pMaterials[0]->SetAlbedo( material["AlbedoColor"].as< DirectX::XMFLOAT3 >() );

                    const auto &metalnessMap = material["MetalnessMap"].as< std::string >();
                    if ( !metalnessMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetMetalness( Texture2D::Create( metalnessMap ) );
                        smc.m_pMaterials[0]->SetUseMetalnessMap( true );
                    }
                    smc.m_pMaterials[0]->SetMetalness( material["Metalness"].as< float >() );

                    const auto &roughnessMap = material["RoughnessMap"].as< std::string >();
                    if ( !roughnessMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetRoughness( Texture2D::Create( roughnessMap ) );
                        smc.m_pMaterials[0]->SetUseRoughnessMap( true );
                    }
                    smc.m_pMaterials[0]->SetRoughness( material["Roughness"].as< float >() );

                    const auto &normalMap = material["NormalMap"].as< std::string >();
                    if ( !normalMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetNormalMap( Texture2D::Create( normalMap ) );
                        smc.m_pMaterials[0]->SetUseNormalMap( true );
                    }

                    const auto &aoMap = material["AOMap"].as< std::string >();
                    if ( !aoMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetAOMap( Texture2D::Create( aoMap ) );
                        smc.m_pMaterials[0]->SetUseAOMap( true );
                    }
                }

                auto skinnedMeshComponent = entity["SkinnedMeshComponent"];
                if ( skinnedMeshComponent )
                {
                    auto &smc = deserializedEntity.AddComponent< SkinnedMeshComponent >();

                    const auto &meshPath = skinnedMeshComponent["Mesh"].as< std::string >();
                    if ( !meshPath.empty() )
                    {
                        smc.m_pMeshes = MeshLoader::LoadSkinnedMesh( meshPath );
                        const auto &bufferLayout = smc.m_pMaterials[0]->GetBufferLayout();
                        for ( const auto &pMesh : smc.m_pMeshes )
                        {
                            pMesh->Create( bufferLayout );

                            if ( pMesh->HasAnimations() )
                            {
                                MeshAnimator animator{ pMesh };
                                smc.m_Animators.push_back( animator );
                                smc.m_Animators.back().SetAnimation( 0 );
                            }
                        }
                    }

                    auto material = skinnedMeshComponent["Material"];

                    const auto &albedoMap = material["AlbedoMap"].as< std::string >();
                    if ( !albedoMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetAlbedo( Texture2D::Create( albedoMap ) );
                        smc.m_pMaterials[0]->SetUseAlbedoMap( true );
                    }
                    smc.m_pMaterials[0]->SetAlbedo( material["AlbedoColor"].as< DirectX::XMFLOAT3 >() );

                    const auto &metalnessMap = material["MetalnessMap"].as< std::string >();
                    if ( !metalnessMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetMetalness( Texture2D::Create( metalnessMap ) );
                        smc.m_pMaterials[0]->SetUseMetalnessMap( true );
                    }
                    smc.m_pMaterials[0]->SetMetalness( material["Metalness"].as< float >() );

                    const auto &roughnessMap = material["RoughnessMap"].as< std::string >();
                    if ( !roughnessMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetRoughness( Texture2D::Create( roughnessMap ) );
                        smc.m_pMaterials[0]->SetUseRoughnessMap( true );
                    }
                    smc.m_pMaterials[0]->SetRoughness( material["Roughness"].as< float >() );

                    const auto &normalMap = material["NormalMap"].as< std::string >();
                    if ( !normalMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetNormalMap( Texture2D::Create( normalMap ) );
                        smc.m_pMaterials[0]->SetUseNormalMap( true );
                    }

                    const auto &aoMap = material["AOMap"].as< std::string >();
                    if ( !aoMap.empty() )
                    {
                        smc.m_pMaterials[0]->SetAOMap( Texture2D::Create( aoMap ) );
                        smc.m_pMaterials[0]->SetUseAOMap( true );
                    }
                }

                auto rigidbodyComponent = entity["RigidbodyComponent"];
                if ( rigidbodyComponent )
                {
                    auto &rbc = deserializedEntity.AddComponent< RigidbodyComponent >();

                    rbc.m_BodyType =
                        static_cast< RigidbodyComponent::BodyType >( rigidbodyComponent["BodyType"].as< int >() );
                    rbc.m_CollisionDetectionType = static_cast< RigidbodyComponent::CollisionDetectionType >(
                        rigidbodyComponent["CollisionDetectionType"].as< int >() );

                    auto physicsMaterial = rigidbodyComponent["PhysicsMaterial"];
                    /*rbc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    rbc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    rbc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

                    rbc.m_Mass = rigidbodyComponent["Mass"].as< float >();
                    rbc.m_LinearDrag = rigidbodyComponent["LinearDrag"].as< float >();
                    rbc.m_AngularDrag = rigidbodyComponent["AngularDrag"].as< float >();

                    rbc.m_bDisableGravity = rigidbodyComponent["bDisableGravity"].as< bool >();
                    rbc.m_bKinematic = rigidbodyComponent["bKinematic"].as< bool >();

                    rbc.m_bLockPositionX = rigidbodyComponent["bLockPositionX"].as< bool >();
                    rbc.m_bLockPositionY = rigidbodyComponent["bLockPositionY"].as< bool >();
                    rbc.m_bLockPositionZ = rigidbodyComponent["bLockPositionZ"].as< bool >();

                    rbc.m_bLockRotationX = rigidbodyComponent["bLockRotationX"].as< bool >();
                    rbc.m_bLockRotationY = rigidbodyComponent["bLockRotationY"].as< bool >();
                    rbc.m_bLockRotationZ = rigidbodyComponent["bLockRotationZ"].as< bool >();
                }

                auto boxColliderComponent = entity["BoxColliderComponent"];
                if ( boxColliderComponent )
                {
                    auto &bcc = deserializedEntity.AddComponent< BoxColliderComponent >();

                    bcc.m_Size = boxColliderComponent["Size"].as< DirectX::XMFLOAT3 >();
                    bcc.m_Offset = boxColliderComponent["Offset"].as< DirectX::XMFLOAT3 >();
                    bcc.m_bTrigger = boxColliderComponent["bTrigger"].as< bool >();
                    bcc.m_bShowColliderBounds = boxColliderComponent["bShowColliderBounds"].as< bool >();

                    /*auto physicsMaterial = rigidBodyComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto sphereColliderComponent = entity["SphereColliderComponent"];
                if ( sphereColliderComponent )
                {
                    auto &scc = deserializedEntity.AddComponent< SphereColliderComponent >();

                    scc.m_Radius = sphereColliderComponent["Radius"].as< float >();
                    scc.m_bTrigger = sphereColliderComponent["bTrigger"].as< bool >();
                    scc.m_bShowColliderBounds = sphereColliderComponent["bShowColliderBounds"].as< bool >();

                    /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto capsuleColliderComponent = entity["CapsuleColliderComponent"];
                if ( capsuleColliderComponent )
                {
                    auto &ccc = deserializedEntity.AddComponent< CapsuleColliderComponent >();

                    ccc.m_Radius = capsuleColliderComponent["Radius"].as< float >();
                    ccc.m_Height = capsuleColliderComponent["Height"].as< float >();
                    ccc.m_bTrigger = capsuleColliderComponent["bTrigger"].as< bool >();
                    ccc.m_bShowColliderBounds = capsuleColliderComponent["bShowColliderBounds"].as< bool >();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime( const std::string &filePath )
    {
        SM_ASSERT( false, "SceneSerializer::DeserializeRuntime > Not implemented" );
        return false;
    }
}