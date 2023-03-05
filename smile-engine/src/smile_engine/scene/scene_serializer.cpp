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
    struct convert< DirectX::XMFLOAT2 >
    {
        static Node encode( const DirectX::XMFLOAT2 &v )
        {
            Node node{};
            node.push_back( v.x );
            node.push_back( v.y );
            return node;
        }

        static bool decode( const Node &node, DirectX::XMFLOAT2 &v )
        {
            if ( !node.IsSequence() || node.size() != 2 )
                return false;

            v.x = node[0].as< float >();
            v.y = node[1].as< float >();
            return true;
        }
    };

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

namespace smile::scene
{
    YAML::Emitter &operator<<( YAML::Emitter &output, const DirectX::XMFLOAT2 &v )
    {
        output << YAML::Flow;
        output << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return output;
    }

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

        m_pScene->m_ECSEngine.Each(
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

    static void SerializeMaterial( YAML::Emitter &output, const Ref< graphic::Material > &pMaterial )
    {
        output << YAML::Key << "Material";
        output << YAML::BeginMap;

        output << YAML::Key << "FloatValues";
        output << YAML::BeginMap;
        const auto &floatValues{ pMaterial->GetFloatValues() };
        for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "IntValues";
        output << YAML::BeginMap;
        const auto &intValues{ pMaterial->GetIntValues() };
        for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "BoolValues";
        output << YAML::BeginMap;
        const auto &boolValues{ pMaterial->GetBoolValues() };
        for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Float2Values";
        output << YAML::BeginMap;
        const auto &float2Values{ pMaterial->GetFloat2Values() };
        for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Float3Values";
        output << YAML::BeginMap;
        const auto &float3Values{ pMaterial->GetFloat3Values() };
        for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Texture2DValues";
        output << YAML::BeginMap;
        const auto &texture2DValues{ pMaterial->GetTexture2DValues() };
        for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value
                   << ( ( *it ).second ? ( *it ).second->GetFilePath() : "" );
        }
        output << YAML::EndMap;

        output << YAML::EndMap;
    }

    static void SerializeEntity( YAML::Emitter &output, Entity entity )
    {
        SM_ASSERT( entity.HasComponent< IDComponent >(),
            "SceneSerializer::serializeScene > Entity does not have an IDComponent" );

        output << YAML::BeginMap;
        output << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        if ( entity.HasComponent< TagComponent >() )
        {
            output << YAML::Key << "TagComponent";
            output << YAML::BeginMap;

            auto &tag = entity.GetComponent< TagComponent >().Tag;
            output << YAML::Key << "Tag" << YAML::Value << tag;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< TransformComponent >() )
        {
            output << YAML::Key << "TransformComponent";
            output << YAML::BeginMap;

            auto &transformComponent = entity.GetComponent< TransformComponent >();
            output << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
            output << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
            output << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< CameraComponent >() )
        {
            output << YAML::Key << "CameraComponent";
            output << YAML::BeginMap;

            auto &cameraComponent = entity.GetComponent< CameraComponent >();
            auto &camera = cameraComponent.Camera;
            output << YAML::Key << "Camera" << YAML::Value;
            output << YAML::BeginMap;

            output << YAML::Key << "ProjectionType" << YAML::Value
                   << static_cast< Uint32 >( camera.GetProjectionType() );
            output << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
            output << YAML::Key << "PerspectiveNearPlane" << YAML::Value << camera.GetPerspectiveNearPlane();
            output << YAML::Key << "PerspectiveFarPlane" << YAML::Value << camera.GetPerspectiveFarPlane();
            output << YAML::Key << "Size" << YAML::Value << camera.GetSize();
            output << YAML::Key << "OrthographicNearPlane" << camera.GetOrthographicNearPlane();
            output << YAML::Key << "OrthographicFarPlane" << camera.GetOrthographicFarPlane();

            output << YAML::EndMap;

            output << YAML::Key << "bPrimary" << YAML::Value << cameraComponent.IsPrimary;
            output << YAML::Key << "bFixedAspectRatio" << YAML::Value << cameraComponent.HasFixedAspectRatio;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< StaticMeshComponent >() )
        {
            output << YAML::Key << "SkinnedMeshComponent";
            output << YAML::BeginMap;

            auto &staticMeshComponent = entity.GetComponent< StaticMeshComponent >();
            output << YAML::Key << "Mesh" << YAML::Value
                   << ( ( staticMeshComponent.pMeshes.size() > 0 ) ? staticMeshComponent.pMeshes[0]->GetFilePath()
                                                                    : "" );

            SerializeMaterial( output, staticMeshComponent.pMaterials[0] );

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< SkinnedMeshComponent >() )
        {
            output << YAML::Key << "SkinnedMeshComponent";
            output << YAML::BeginMap;

            auto &skinnedMeshComponent = entity.GetComponent< SkinnedMeshComponent >();
            output << YAML::Key << "Mesh" << YAML::Value
                   << ( ( skinnedMeshComponent.pMeshes.size() > 0 ) ? skinnedMeshComponent.pMeshes[0]->GetFilePath()
                                                                     : "" );

            SerializeMaterial( output, skinnedMeshComponent.pMaterials[0] );

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< RigidbodyComponent >() )
        {
            output << YAML::Key << "RigidbodyComponent";
            output << YAML::BeginMap;

            auto &rigidbodyComponent = entity.GetComponent< RigidbodyComponent >();
            output << YAML::Key << "BodyType" << YAML::Value << static_cast< Uint32 >( rigidbodyComponent.Type );
            output << YAML::Key << "CollisionDetectionType" << YAML::Value
                   << static_cast< Uint32 >( rigidbodyComponent.CollisionDetection );

            output << YAML::Key << "PhysicsMaterial";
            output << YAML::BeginMap;

            /*auto& pPhysicsMaterial = rigidbodyComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;

            output << YAML::Key << "Mass" << YAML::Value << rigidbodyComponent.Mass;
            output << YAML::Key << "LinearDrag" << YAML::Value << rigidbodyComponent.LinearDrag;
            output << YAML::Key << "AngularDrag" << YAML::Value << rigidbodyComponent.AngularDrag;
            output << YAML::Key << "bDisableGravity" << YAML::Value << rigidbodyComponent.DisableGravity;
            output << YAML::Key << "bKinematic" << YAML::Value << rigidbodyComponent.IsKinematic;

            output << YAML::Key << "bLockPositionX" << YAML::Value << rigidbodyComponent.LockPositionX;
            output << YAML::Key << "bLockPositionY" << YAML::Value << rigidbodyComponent.LockPositionY;
            output << YAML::Key << "bLockPositionZ" << YAML::Value << rigidbodyComponent.LockPositionZ;

            output << YAML::Key << "bLockRotationX" << YAML::Value << rigidbodyComponent.LockRotationX;
            output << YAML::Key << "bLockRotationY" << YAML::Value << rigidbodyComponent.LockRotationY;
            output << YAML::Key << "bLockRotationZ" << YAML::Value << rigidbodyComponent.LockRotationZ;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< BoxColliderComponent >() )
        {
            output << YAML::Key << "BoxColliderComponent";
            output << YAML::BeginMap;

            auto &boxColliderComponent = entity.GetComponent< BoxColliderComponent >();
            output << YAML::Key << "Size" << YAML::Value << boxColliderComponent.Size;
            output << YAML::Key << "Offset" << YAML::Value << boxColliderComponent.Offset;
            output << YAML::Key << "bTrigger" << YAML::Value << boxColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << boxColliderComponent.ShowColliderBounds;

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
            output << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent.Radius;
            output << YAML::Key << "bTrigger" << YAML::Value << sphereColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << sphereColliderComponent.ShowColliderBounds;

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
            output << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent.Radius;
            output << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent.Height;
            output << YAML::Key << "bTrigger" << YAML::Value << capsuleColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value
                   << capsuleColliderComponent.ShowColliderBounds;

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
                uint64_t uuid = entity["Entity"].as< Uint64 >();

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
                    tc.Translation = transformComponent["Translation"].as< DirectX::XMFLOAT3 >();
                    tc.Rotation = transformComponent["Rotation"].as< DirectX::XMFLOAT3 >();
                    tc.Scale = transformComponent["Scale"].as< DirectX::XMFLOAT3 >();
                }

                auto cameraComponent = entity["CameraComponent"];
                if ( cameraComponent )
                {
                    auto &cc = deserializedEntity.AddComponent< CameraComponent >();

                    auto cameraProps = cameraComponent["Camera"];
                    cc.Camera.SetProjectionType(
                        static_cast< SceneCamera::ProjectionType >( cameraProps["ProjectionType"].as< int >() ) );

                    cc.Camera.SetFOV( cameraProps["FOV"].as< float >() );
                    cc.Camera.SetPerspectiveNearPlane( cameraProps["PerspectiveNearPlane"].as< float >() );
                    cc.Camera.SetPerspectiveFarPlane( cameraProps["PerspectiveFarPlane"].as< float >() );

                    cc.Camera.SetSize( cameraProps["Size"].as< float >() );
                    cc.Camera.SetOrthographicNearPlane( cameraProps["OrthographicNearPlane"].as< float >() );
                    cc.Camera.SetOrthographicFarPlane( cameraProps["OrthographicFarPlane"].as< float >() );

                    cc.IsPrimary = cameraComponent["bPrimary"].as< bool >();
                    cc.HasFixedAspectRatio = cameraComponent["bFixedAspectRatio"].as< bool >();
                }

                auto staticMeshComponent = entity["StaticMeshComponent"];
                if ( staticMeshComponent )
                {
                    auto &smc = deserializedEntity.AddComponent< StaticMeshComponent >();

                    const auto &meshPath = staticMeshComponent["Mesh"].as< std::string >();
                    if ( !meshPath.empty() )
                    {
                        smc.pMeshes = graphic::MeshLoader::LoadStaticMesh( meshPath );
                        const auto &bufferLayout = smc.pMaterials[0]->GetBufferLayout();
                        for ( const auto &pMesh : smc.pMeshes )
                        {
                            pMesh->Create( bufferLayout );
                        }
                    }

                    auto material = staticMeshComponent["Material"];

                    auto floatValues = material["FloatValues"];
                    for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        smc.pMaterials[0]->SetFloatValue( semantic, value );
                    }

                    auto intValues = material["IntValues"];
                    for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        smc.pMaterials[0]->SetIntValue( semantic, value );
                    }

                    auto boolValues = material["BoolValues"];
                    for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        smc.pMaterials[0]->SetBoolValue( semantic, value );
                    }

                    auto float2Values = material["Float2Values"];
                    for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        smc.pMaterials[0]->SetFloat2Value( semantic, value );
                    }

                    auto float3Values = material["Float3Values"];
                    for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        smc.pMaterials[0]->SetFloat3Value( semantic, value );
                    }

                    auto texture2DValues = material["Texture2DValues"];
                    for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto path = ( *it ).second.as< std::string >();
                        if ( !path.empty() )
                            smc.pMaterials[0]->SetTexture2D( semantic, graphic::Texture2D::Create( path ) );
                    }
                }

                auto skinnedMeshComponent = entity["SkinnedMeshComponent"];
                if ( skinnedMeshComponent )
                {
                    auto &smc = deserializedEntity.AddComponent< SkinnedMeshComponent >();

                    const auto &meshPath = skinnedMeshComponent["Mesh"].as< std::string >();
                    if ( !meshPath.empty() )
                    {
                        smc.pMeshes = graphic::MeshLoader::LoadSkinnedMesh( meshPath );
                        const auto &bufferLayout = smc.pMaterials[0]->GetBufferLayout();
                        for ( const auto &mesh : smc.pMeshes )
                        {
                            mesh->Create( bufferLayout );

                            if ( mesh->HasAnimations() )
                            {
                                graphic::MeshAnimator animator{ mesh };
                                smc.Animators.push_back( animator );
                                smc.Animators.back().SetAnimation( 0 );
                            }
                        }
                    }

                    auto material = skinnedMeshComponent["Material"];

                    auto floatValues = material["FloatValues"];
                    for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        smc.pMaterials[0]->SetFloatValue( semantic, value );
                    }

                    auto intValues = material["IntValues"];
                    for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        smc.pMaterials[0]->SetIntValue( semantic, value );
                    }

                    auto boolValues = material["BoolValues"];
                    for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        smc.pMaterials[0]->SetBoolValue( semantic, value );
                    }

                    auto float2Values = material["Float2Values"];
                    for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        smc.pMaterials[0]->SetFloat2Value( semantic, value );
                    }

                    auto float3Values = material["Float3Values"];
                    for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        smc.pMaterials[0]->SetFloat3Value( semantic, value );
                    }

                    auto texture2DValues = material["Texture2DValues"];
                    for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto path = ( *it ).second.as< std::string >();
                        if ( !path.empty() )
                            smc.pMaterials[0]->SetTexture2D( semantic, graphic::Texture2D::Create( path ) );
                    }
                }

                auto rigidbodyComponent = entity["RigidbodyComponent"];
                if ( rigidbodyComponent )
                {
                    auto &rbc = deserializedEntity.AddComponent< RigidbodyComponent >();

                    rbc.Type =
                        static_cast< RigidbodyComponent::BodyType >( rigidbodyComponent["BodyType"].as< int >() );
                    rbc.CollisionDetection = static_cast< RigidbodyComponent::CollisionDetectionType >(
                        rigidbodyComponent["CollisionDetectionType"].as< int >() );

                    auto physicsMaterial = rigidbodyComponent["PhysicsMaterial"];
                    /*rbc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    rbc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    rbc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

                    rbc.Mass = rigidbodyComponent["Mass"].as< float >();
                    rbc.LinearDrag = rigidbodyComponent["LinearDrag"].as< float >();
                    rbc.AngularDrag = rigidbodyComponent["AngularDrag"].as< float >();

                    rbc.DisableGravity = rigidbodyComponent["bDisableGravity"].as< bool >();
                    rbc.IsKinematic = rigidbodyComponent["bKinematic"].as< bool >();

                    rbc.LockPositionX = rigidbodyComponent["bLockPositionX"].as< bool >();
                    rbc.LockPositionY = rigidbodyComponent["bLockPositionY"].as< bool >();
                    rbc.LockPositionZ = rigidbodyComponent["bLockPositionZ"].as< bool >();

                    rbc.LockRotationX = rigidbodyComponent["bLockRotationX"].as< bool >();
                    rbc.LockRotationY = rigidbodyComponent["bLockRotationY"].as< bool >();
                    rbc.LockRotationZ = rigidbodyComponent["bLockRotationZ"].as< bool >();
                }

                auto boxColliderComponent = entity["BoxColliderComponent"];
                if ( boxColliderComponent )
                {
                    auto &bcc = deserializedEntity.AddComponent< BoxColliderComponent >();

                    bcc.Size = boxColliderComponent["Size"].as< DirectX::XMFLOAT3 >();
                    bcc.Offset = boxColliderComponent["Offset"].as< DirectX::XMFLOAT3 >();
                    bcc.IsTrigger = boxColliderComponent["bTrigger"].as< bool >();
                    bcc.ShowColliderBounds = boxColliderComponent["bShowColliderBounds"].as< bool >();

                    /*auto physicsMaterial = rigidBodyComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto sphereColliderComponent = entity["SphereColliderComponent"];
                if ( sphereColliderComponent )
                {
                    auto &scc = deserializedEntity.AddComponent< SphereColliderComponent >();

                    scc.Radius = sphereColliderComponent["Radius"].as< float >();
                    scc.IsTrigger = sphereColliderComponent["bTrigger"].as< bool >();
                    scc.ShowColliderBounds = sphereColliderComponent["bShowColliderBounds"].as< bool >();

                    /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto capsuleColliderComponent = entity["CapsuleColliderComponent"];
                if ( capsuleColliderComponent )
                {
                    auto &ccc = deserializedEntity.AddComponent< CapsuleColliderComponent >();

                    ccc.Radius = capsuleColliderComponent["Radius"].as< float >();
                    ccc.Height = capsuleColliderComponent["Height"].as< float >();
                    ccc.IsTrigger = capsuleColliderComponent["bTrigger"].as< bool >();
                    ccc.ShowColliderBounds = capsuleColliderComponent["bShowColliderBounds"].as< bool >();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime( const std::string &filePath )
    {
        SM_ASSERT( false, "SceneSerializer::deserializeRuntime > Not implemented" );
        return false;
    }
}