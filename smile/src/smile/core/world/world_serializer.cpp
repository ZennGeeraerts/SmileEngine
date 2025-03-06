/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world_serializer.h"

#include "logging/logger.h"
#include "entity.h"
#include "components.h"
#include "project/project_manager.h"

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

namespace smile::world
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

    WorldSerializer::WorldSerializer( Ref< World > pWorld ) : m_pWorld{ pWorld }
    {
    }

    void WorldSerializer::Serialize( const std::string &filePath )
    {
        YAML::Emitter output{};
        output << YAML::BeginMap;
        output << YAML::Key << "World" << YAML::Value << "Untitled";
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_pWorld->m_ECSEngine.Each(
            [&]( auto entityID )
            {
                Entity entity{ entityID, m_pWorld.get() };
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
            if ( ( *it ).second )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ ( *it ).second->FilePath };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << ( *it ).first << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << ( *it ).first << YAML::Value << "";
            }
        }
        output << YAML::EndMap;

        output << YAML::EndMap;
    }

    static void SerializeEntity( YAML::Emitter &output, Entity entity )
    {
        SM_ASSERT( entity.HasComponent< ecs::IDComponent >(),
            "WorldSerializer::SerializeWorld > Entity does not have an IDComponent" );

        output << YAML::BeginMap;
        output << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        if ( entity.HasComponent< ecs::TagComponent >() )
        {
            output << YAML::Key << "TagComponent";
            output << YAML::BeginMap;

            auto &tag = entity.GetComponent< ecs::TagComponent >().Tag;
            output << YAML::Key << "Tag" << YAML::Value << tag;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< ecs::TransformComponent >() )
        {
            output << YAML::Key << "TransformComponent";
            output << YAML::BeginMap;

            auto &transformComponent = entity.GetComponent< ecs::TransformComponent >();
            output << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
            output << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
            output << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::CameraComponent >() )
        {
            output << YAML::Key << "CameraComponent";
            output << YAML::BeginMap;

            auto &cameraComponent = entity.GetComponent< graphic::ecs::CameraComponent >();
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

        if ( entity.HasComponent< scripting::ecs::ScriptComponent >() )
        {
            output << YAML::Key << "ScriptComponent";
            output << YAML::BeginMap;

            auto &scriptComponent = entity.GetComponent< scripting::ecs::ScriptComponent >();
            output << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::MeshRendererComponent >() )
        {
            output << YAML::Key << "MeshRendererComponent";
            output << YAML::BeginMap;

            auto &meshRendererComponent = entity.GetComponent< graphic::ecs::MeshRendererComponent >();

            if ( meshRendererComponent.pModel )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ meshRendererComponent.pModel->GetFilePath() };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Model" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Model" << YAML::Value << "";
            }

            output << YAML::Key << "MeshIndex" << meshRendererComponent.MeshIndex;

            SerializeMaterial( output, meshRendererComponent.pMaterial );

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::SkinnedMeshRendererComponent >() )
        {
            output << YAML::Key << "SkinnedMeshRendererComponent";
            output << YAML::BeginMap;

            auto &skinnedMeshRendererComponent = entity.GetComponent< graphic::ecs::SkinnedMeshRendererComponent >();
            if ( skinnedMeshRendererComponent.pModel )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ skinnedMeshRendererComponent.pModel->GetFilePath() };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Model" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Model" << YAML::Value << "";
            }

            output << YAML::Key << "MeshIndex" << skinnedMeshRendererComponent.MeshIndex;

            SerializeMaterial( output, skinnedMeshRendererComponent.pMaterial );

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::AnimatorComponent >() )
        {
            output << YAML::Key << "AnimatorComponent";
            output << YAML::BeginMap;

            auto &animatorComponent = entity.GetComponent< graphic::ecs::AnimatorComponent >();
            if ( animatorComponent.pModel )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ animatorComponent.pModel->GetFilePath() };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Model" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Model" << YAML::Value << "";
            }

            output << YAML::Key << "ClipIndex" << animatorComponent.CurrentClipIndex;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< physics::ecs::RigidbodyComponent >() )
        {
            output << YAML::Key << "RigidbodyComponent";
            output << YAML::BeginMap;

            auto &rigidbodyComponent = entity.GetComponent< physics::ecs::RigidbodyComponent >();
            output << YAML::Key << "BodyType" << YAML::Value << static_cast< Uint32 >( rigidbodyComponent.BodyType );
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

        if ( entity.HasComponent< physics::ecs::BoxColliderComponent >() )
        {
            output << YAML::Key << "BoxColliderComponent";
            output << YAML::BeginMap;

            auto &boxColliderComponent = entity.GetComponent< physics::ecs::BoxColliderComponent >();
            output << YAML::Key << "Size" << YAML::Value << boxColliderComponent.Box.Size;
            output << YAML::Key << "Offset" << YAML::Value << boxColliderComponent.Box.Center;
            output << YAML::Key << "bTrigger" << YAML::Value << boxColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << boxColliderComponent.ShowColliderBounds;

            /*auto& pPhysicsMaterial = boxColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< physics::ecs::SphereColliderComponent >() )
        {
            output << YAML::Key << "SphereColliderComponent";
            output << YAML::BeginMap;

            auto &sphereColliderComponent = entity.GetComponent< physics::ecs::SphereColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent.Sphere.Radius;
            output << YAML::Key << "bTrigger" << YAML::Value << sphereColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << sphereColliderComponent.ShowColliderBounds;

            /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< physics::ecs::CapsuleColliderComponent >() )
        {
            output << YAML::Key << "CapsuleColliderComponent";
            output << YAML::BeginMap;

            auto &capsuleColliderComponent = entity.GetComponent< physics::ecs::CapsuleColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent.Capsule.Radius;
            output << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent.Capsule.Height;
            output << YAML::Key << "bTrigger" << YAML::Value << capsuleColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << capsuleColliderComponent.ShowColliderBounds;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< physics::ecs::CharacterControllerComponent >() )
        {
            output << YAML::Key << "CharacterControllerComponent";
            output << YAML::BeginMap;

            const auto &characterControllerComponent =
                entity.GetComponent< physics::ecs::CharacterControllerComponent >();
            output << YAML::Key << "Radius" << YAML::Value << characterControllerComponent.Radius;
            output << YAML::Key << "Height" << YAML::Value << characterControllerComponent.Height;
            output << YAML::Key << "ClimbingMode" << YAML::Value
                   << static_cast< Uint32 >( characterControllerComponent.ClimbingMode );
            output << YAML::Key << "Name" << YAML::Value << characterControllerComponent.Name;
            output << YAML::Key << "CollisionGroups" << YAML::Value
                   << static_cast< Uint32 >( characterControllerComponent.CollisionFlags );
            output << YAML::Key << "CollisionIgnoreGroups" << YAML::Value
                   << static_cast< Uint32 >( characterControllerComponent.CollisionIgnoreGroups );
            output << YAML::Key << "CollisionFlags" << YAML::Value
                   << static_cast< Uint32 >( characterControllerComponent.CollisionFlags );

            /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< graphic::ecs::SpriteRendererComponent >() )
        {
            output << YAML::Key << "SpriteRendererComponent";
            output << YAML::BeginMap;

            auto &spriteRendererComponent = entity.GetComponent< graphic::ecs::SpriteRendererComponent >();
            output << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

            if ( spriteRendererComponent.pTexture )
            {
                auto basePath = project::ProjectManager::GetActive()->GetAssetDirectory();
                auto fullPath = std::filesystem::path{ spriteRendererComponent.pTexture->FilePath };
                auto relativePath = fullPath.lexically_relative( basePath );

                output << YAML::Key << "Texture" << YAML::Value << relativePath.string();
            }
            else
            {
                output << YAML::Key << "Texture" << YAML::Value << "";
            }
        }

        output << YAML::EndMap;
    }

    void WorldSerializer::SerializeRuntime( const std::string &filePath )
    {
        SM_ASSERT( false, "WorldSerializer::SerializeRuntime > Not implemented" );
    }

    bool WorldSerializer::Deserialize( const std::string &filePath )
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile( filePath );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_ERROR( "Failed to load .smile file: {0}\n{1}", filePath, e.what() );
            return false;
        }

        if ( !data["World"] )
            return false;

        std::string worldName = data["World"].as< std::string >();
        SM_LOG_TRACE( "Deserializing world '{}'", worldName );

        auto entities = data["Entities"];
        if ( entities )
        {
            for ( auto entity : entities )
            {
                Uint64 uuid = entity["Entity"].as< Uint64 >();

                std::string name{};
                auto tagComponent = entity["TagComponent"];
                if ( tagComponent )
                    name = tagComponent["Tag"].as< std::string >();

                SM_LOG_TRACE( "Deserialized entity with ID: {0}, name: {1}", uuid, name );

                Entity deserializedEntity = m_pWorld->CreateEntity( uuid, name );

                auto transformComponent = entity["TransformComponent"];
                if ( transformComponent )
                {
                    auto &tc = deserializedEntity.GetComponent< ecs::TransformComponent >();
                    tc.Translation = transformComponent["Translation"].as< DirectX::XMFLOAT3 >();
                    tc.Rotation = transformComponent["Rotation"].as< DirectX::XMFLOAT3 >();
                    tc.Scale = transformComponent["Scale"].as< DirectX::XMFLOAT3 >();
                }

                auto cameraComponent = entity["CameraComponent"];
                if ( cameraComponent )
                {
                    auto &cc = deserializedEntity.AddComponent< graphic::ecs::CameraComponent >();

                    auto cameraProps = cameraComponent["Camera"];
                    cc.Camera.SetProjectionType( static_cast< graphic::SceneCamera::ProjectionType >(
                        cameraProps["ProjectionType"].as< int >() ) );

                    cc.Camera.SetFOV( cameraProps["FOV"].as< float >() );
                    cc.Camera.SetPerspectiveNearPlane( cameraProps["PerspectiveNearPlane"].as< float >() );
                    cc.Camera.SetPerspectiveFarPlane( cameraProps["PerspectiveFarPlane"].as< float >() );

                    cc.Camera.SetSize( cameraProps["Size"].as< float >() );
                    cc.Camera.SetOrthographicNearPlane( cameraProps["OrthographicNearPlane"].as< float >() );
                    cc.Camera.SetOrthographicFarPlane( cameraProps["OrthographicFarPlane"].as< float >() );

                    cc.IsPrimary = cameraComponent["bPrimary"].as< bool >();
                    cc.HasFixedAspectRatio = cameraComponent["bFixedAspectRatio"].as< bool >();
                }

                auto scriptComponent = entity["ScriptComponent"];
                if ( scriptComponent )
                {
                    auto &sc = deserializedEntity.AddComponent< scripting::ecs::ScriptComponent >();
                    sc.ClassName = scriptComponent["ClassName"].as< std::string >();
                }

                auto meshRendererComponent = entity["MeshRendererComponent"];
                if ( meshRendererComponent )
                {
                    auto &mrc = deserializedEntity.AddComponent< graphic::ecs::MeshRendererComponent >();

                    mrc.MeshIndex = meshRendererComponent["MeshIndex"].as< Uint32 >();

                    auto modelPath = meshRendererComponent["Model"].as< std::string >();
                    if ( !modelPath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                        mrc.pModel = graphic::ModelLoader::LoadModel( path.string() );

                        auto pMeshFilter = mrc.pModel->GetMeshFilter( mrc.MeshIndex );
                        mrc.pMesh = graphic::MeshFactory::CreateMesh( pMeshFilter, mrc.pMaterial->GetBufferLayout() );
                    }

                    auto material = meshRendererComponent["Material"];

                    auto floatValues = material["FloatValues"];
                    for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        mrc.pMaterial->SetFloatValue( semantic, value );
                    }

                    auto intValues = material["IntValues"];
                    for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        mrc.pMaterial->SetIntValue( semantic, value );
                    }

                    auto boolValues = material["BoolValues"];
                    for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        mrc.pMaterial->SetBoolValue( semantic, value );
                    }

                    auto float2Values = material["Float2Values"];
                    for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        mrc.pMaterial->SetFloat2Value( semantic, value );
                    }

                    auto float3Values = material["Float3Values"];
                    for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        mrc.pMaterial->SetFloat3Value( semantic, value );
                    }

                    auto texture2DValues = material["Texture2DValues"];
                    for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto texturePath = ( *it ).second.as< std::string >();
                        if ( !texturePath.empty() )
                        {
                            auto path = project::ProjectManager::GetAssetFileSystemPath( texturePath );
                            mrc.pMaterial->SetTexture2D( semantic,
                                graphic::RenderCommand::GetGraphicsDevice()->CreateTexture2D( path.string() ) );
                        }
                    }
                }

                auto skinnedMeshRendererComponent = entity["SkinnedMeshRendererComponent"];
                if ( skinnedMeshRendererComponent )
                {
                    auto &smrc = deserializedEntity.AddComponent< graphic::ecs::SkinnedMeshRendererComponent >();

                    smrc.MeshIndex = skinnedMeshRendererComponent["MeshIndex"].as< Uint32 >();

                    auto modelPath = skinnedMeshRendererComponent["Model"].as< std::string >();
                    if ( !modelPath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                        smrc.pModel = graphic::ModelLoader::LoadModel( path.string() );

                        auto pSkinnedMeshFilter = smrc.pModel->GetSkinnedMeshFilter( smrc.MeshIndex );
                        smrc.pSkinnedMesh = graphic::MeshFactory::CreateSkinnedMesh(
                            pSkinnedMeshFilter, smrc.pMaterial->GetBufferLayout() );
                    }

                    auto material = skinnedMeshRendererComponent["Material"];

                    auto floatValues = material["FloatValues"];
                    for ( auto it{ floatValues.begin() }; it != floatValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        smrc.pMaterial->SetFloatValue( semantic, value );
                    }

                    auto intValues = material["IntValues"];
                    for ( auto it{ intValues.begin() }; it != intValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        smrc.pMaterial->SetIntValue( semantic, value );
                    }

                    auto boolValues = material["BoolValues"];
                    for ( auto it{ boolValues.begin() }; it != boolValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        smrc.pMaterial->SetBoolValue( semantic, value );
                    }

                    auto float2Values = material["Float2Values"];
                    for ( auto it{ float2Values.begin() }; it != float2Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        smrc.pMaterial->SetFloat2Value( semantic, value );
                    }

                    auto float3Values = material["Float3Values"];
                    for ( auto it{ float3Values.begin() }; it != float3Values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        smrc.pMaterial->SetFloat3Value( semantic, value );
                    }

                    auto texture2DValues = material["Texture2DValues"];
                    for ( auto it{ texture2DValues.begin() }; it != texture2DValues.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto texturePath = ( *it ).second.as< std::string >();
                        if ( !texturePath.empty() )
                        {
                            auto path = project::ProjectManager::GetAssetFileSystemPath( texturePath );
                            smrc.pMaterial->SetTexture2D( semantic,
                                graphic::RenderCommand::GetGraphicsDevice()->CreateTexture2D( path.string() ) );
                        }
                    }
                }

                auto animatorComponent = entity["AnimatorComponent"];
                if ( animatorComponent )
                {
                    auto &ac = deserializedEntity.AddComponent< graphic::ecs::AnimatorComponent >();

                    ac.CurrentClipIndex = animatorComponent["ClipIndex"].as< Uint32 >();

                    auto modelPath = animatorComponent["Model"].as< std::string >();
                    if ( !modelPath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( modelPath );
                        ac.pModel = graphic::ModelLoader::LoadModel( path.string() );
                        ac.pAnimationClips = ac.pModel->GetAnimationClips();
                    }
                }

                auto rigidbodyComponent = entity["RigidbodyComponent"];
                if ( rigidbodyComponent )
                {
                    auto &rbc = deserializedEntity.AddComponent< physics::ecs::RigidbodyComponent >();

                    rbc.BodyType = static_cast< physics::RigidbodyType >( rigidbodyComponent["BodyType"].as< int >() );
                    rbc.CollisionDetection = static_cast< physics::CollisionDetectionType >(
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
                    auto &bcc = deserializedEntity.AddComponent< physics::ecs::BoxColliderComponent >();

                    bcc.Box.Size = boxColliderComponent["Size"].as< DirectX::XMFLOAT3 >();
                    bcc.Box.Center = boxColliderComponent["Offset"].as< DirectX::XMFLOAT3 >();
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
                    auto &scc = deserializedEntity.AddComponent< physics::ecs::SphereColliderComponent >();

                    scc.Sphere.Radius = sphereColliderComponent["Radius"].as< float >();
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
                    auto &ccc = deserializedEntity.AddComponent< physics::ecs::CapsuleColliderComponent >();

                    ccc.Capsule.Radius = capsuleColliderComponent["Radius"].as< float >();
                    ccc.Capsule.Height = capsuleColliderComponent["Height"].as< float >();
                    ccc.IsTrigger = capsuleColliderComponent["bTrigger"].as< bool >();
                    ccc.ShowColliderBounds = capsuleColliderComponent["bShowColliderBounds"].as< bool >();
                }

                auto characterControllerComponent = entity["CharacterControllerComponent"];
                if ( characterControllerComponent )
                {
                    auto &ccc = deserializedEntity.AddComponent< physics::ecs::CharacterControllerComponent >();

                    ccc.Radius = characterControllerComponent["Radius"].as< float >();
                    ccc.Height = characterControllerComponent["Height"].as< float >();
                    ccc.ClimbingMode = static_cast< physics::CharacterController::ClimbingModeType >(
                        characterControllerComponent["ClimbingMode"].as< Uint32 >() );
                    ccc.Name = characterControllerComponent["Name"].as< std::string >();
                    ccc.CollisionGroups = static_cast< physics::CollisionGroupFlag >(
                        characterControllerComponent["CollisionGroups"].as< Uint32 >() );
                    ccc.CollisionIgnoreGroups = static_cast< physics::CollisionGroupFlag >(
                        characterControllerComponent["CollisionIgnoreGroups"].as< Uint32 >() );
                    ccc.CollisionFlags = static_cast< physics::CharacterController::CollisionFlag >(
                        characterControllerComponent["CollisionFlags"].as< Uint32 >() );

                    /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if ( spriteRendererComponent )
                {
                    auto &src = deserializedEntity.AddComponent< graphic::ecs::SpriteRendererComponent >();

                    src.Color = spriteRendererComponent["Color"].as< DirectX::XMFLOAT4 >();
                    auto texturePath = spriteRendererComponent["Texture"].as< std::string >();
                    if ( !texturePath.empty() )
                    {
                        auto path = project::ProjectManager::GetAssetFileSystemPath( texturePath );
                        src.pTexture = graphic::RenderCommand::GetGraphicsDevice()->CreateTexture2D( path.string() );
                    }
                }
            }
        }

        return true;
    }

    bool WorldSerializer::DeserializeRuntime( const std::string &filePath )
    {
        SM_ASSERT( false, "WorldSerializer::DeserializeRuntime > Not implemented" );
        return false;
    }
}