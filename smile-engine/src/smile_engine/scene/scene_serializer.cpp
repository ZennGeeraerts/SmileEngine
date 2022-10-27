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

    SceneSerializer::SceneSerializer( const Ref< Scene > &scene ) : scene{ scene }
    {
    }

    void SceneSerializer::serialize( const std::string &file_path )
    {
        YAML::Emitter output{};
        output << YAML::BeginMap;
        output << YAML::Key << "Scene" << YAML::Value << "Untitled";
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        scene->ecsEngine.each(
            [&]( auto entity_id )
            {
                Entity entity{ entity_id, scene.get() };
                if ( !entity )
                    return;
                serializeEntity( output, entity );
            } );

        output << YAML::EndSeq;
        output << YAML::EndMap;

        std::ofstream file_output{ file_path };
        file_output << output.c_str();
    }

    static void serializeMaterial( YAML::Emitter &output, const Ref< graphic::Material > &material )
    {
        output << YAML::Key << "Material";
        output << YAML::BeginMap;

        output << YAML::Key << "FloatValues";
        output << YAML::BeginMap;
        const auto &float_values{ material->getFloatValues() };
        for ( auto it{ float_values.begin() }; it != float_values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "IntValues";
        output << YAML::BeginMap;
        const auto &int_values{ material->getIntValues() };
        for ( auto it{ int_values.begin() }; it != int_values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "BoolValues";
        output << YAML::BeginMap;
        const auto &bool_values{ material->getBoolValues() };
        for ( auto it{ bool_values.begin() }; it != bool_values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Float2Values";
        output << YAML::BeginMap;
        const auto &float2_values{ material->getFloat2Values() };
        for ( auto it{ float2_values.begin() }; it != float2_values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Float3Values";
        output << YAML::BeginMap;
        const auto &float3_values{ material->getFloat3Values() };
        for ( auto it{ float3_values.begin() }; it != float3_values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value << ( *it ).second;
        }
        output << YAML::EndMap;

        output << YAML::Key << "Texture2DValues";
        output << YAML::BeginMap;
        const auto &texture_2d_values{ material->getTexture2DValues() };
        for ( auto it{ texture_2d_values.begin() }; it != texture_2d_values.end(); ++it )
        {
            output << YAML::Key << ( *it ).first << YAML::Value
                   << ( ( *it ).second ? ( *it ).second->getFilePath() : "" );
        }
        output << YAML::EndMap;

        output << YAML::EndMap;
    }

    static void serializeEntity( YAML::Emitter &output, Entity entity )
    {
        SM_ASSERT( entity.hasComponent< IDComponent >(),
            "SceneSerializer::serializeScene > Entity does not have an IDComponent" );

        output << YAML::BeginMap;
        output << YAML::Key << "Entity" << YAML::Value << entity.getUUID();

        if ( entity.hasComponent< TagComponent >() )
        {
            output << YAML::Key << "TagComponent";
            output << YAML::BeginMap;

            auto &tag = entity.getComponent< TagComponent >().tag;
            output << YAML::Key << "Tag" << YAML::Value << tag;

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< TransformComponent >() )
        {
            output << YAML::Key << "TransformComponent";
            output << YAML::BeginMap;

            auto &transform_component = entity.getComponent< TransformComponent >();
            output << YAML::Key << "Translation" << YAML::Value << transform_component.translation;
            output << YAML::Key << "Rotation" << YAML::Value << transform_component.rotation;
            output << YAML::Key << "Scale" << YAML::Value << transform_component.scale;

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< CameraComponent >() )
        {
            output << YAML::Key << "CameraComponent";
            output << YAML::BeginMap;

            auto &camera_component = entity.getComponent< CameraComponent >();
            auto &camera = camera_component.camera;
            output << YAML::Key << "Camera" << YAML::Value;
            output << YAML::BeginMap;

            output << YAML::Key << "ProjectionType" << YAML::Value
                   << static_cast< Uint32 >( camera.getProjectionType() );
            output << YAML::Key << "FOV" << YAML::Value << camera.getFOV();
            output << YAML::Key << "PerspectiveNearPlane" << YAML::Value << camera.getPerspectiveNearPlane();
            output << YAML::Key << "PerspectiveFarPlane" << YAML::Value << camera.getPerspectiveFarPlane();
            output << YAML::Key << "Size" << YAML::Value << camera.getSize();
            output << YAML::Key << "OrthographicNearPlane" << camera.getOrthographicNearPlane();
            output << YAML::Key << "OrthographicFarPlane" << camera.getOrthographicFarPlane();

            output << YAML::EndMap;

            output << YAML::Key << "bPrimary" << YAML::Value << camera_component.primary;
            output << YAML::Key << "bFixedAspectRatio" << YAML::Value << camera_component.fixedAspectRatio;

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< StaticMeshComponent >() )
        {
            output << YAML::Key << "SkinnedMeshComponent";
            output << YAML::BeginMap;

            auto &static_mesh_component = entity.getComponent< StaticMeshComponent >();
            output << YAML::Key << "Mesh" << YAML::Value
                   << ( ( static_mesh_component.meshes.size() > 0 ) ? static_mesh_component.meshes[0]->getFilePath()
                                                                    : "" );

            serializeMaterial( output, static_mesh_component.materials[0] );

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< SkinnedMeshComponent >() )
        {
            output << YAML::Key << "SkinnedMeshComponent";
            output << YAML::BeginMap;

            auto &skinned_mesh_component = entity.getComponent< SkinnedMeshComponent >();
            output << YAML::Key << "Mesh" << YAML::Value
                   << ( ( skinned_mesh_component.meshes.size() > 0 ) ? skinned_mesh_component.meshes[0]->getFilePath()
                                                                     : "" );

            serializeMaterial( output, skinned_mesh_component.materials[0] );

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< RigidbodyComponent >() )
        {
            output << YAML::Key << "RigidbodyComponent";
            output << YAML::BeginMap;

            auto &rigidbody_component = entity.getComponent< RigidbodyComponent >();
            output << YAML::Key << "BodyType" << YAML::Value << static_cast< Uint32 >( rigidbody_component.bodyType );
            output << YAML::Key << "CollisionDetectionType" << YAML::Value
                   << static_cast< Uint32 >( rigidbody_component.collisionDetectionType );

            output << YAML::Key << "PhysicsMaterial";
            output << YAML::BeginMap;

            /*auto& pPhysicsMaterial = rigidbodyComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;

            output << YAML::Key << "Mass" << YAML::Value << rigidbody_component.mass;
            output << YAML::Key << "LinearDrag" << YAML::Value << rigidbody_component.linearDrag;
            output << YAML::Key << "AngularDrag" << YAML::Value << rigidbody_component.angularDrag;
            output << YAML::Key << "bDisableGravity" << YAML::Value << rigidbody_component.disableGravity;
            output << YAML::Key << "bKinematic" << YAML::Value << rigidbody_component.kinematic;

            output << YAML::Key << "bLockPositionX" << YAML::Value << rigidbody_component.lockPositionX;
            output << YAML::Key << "bLockPositionY" << YAML::Value << rigidbody_component.lockPositionY;
            output << YAML::Key << "bLockPositionZ" << YAML::Value << rigidbody_component.lockPositionZ;

            output << YAML::Key << "bLockRotationX" << YAML::Value << rigidbody_component.lockRotationX;
            output << YAML::Key << "bLockRotationY" << YAML::Value << rigidbody_component.lockRotationY;
            output << YAML::Key << "bLockRotationZ" << YAML::Value << rigidbody_component.lockRotationZ;

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< BoxColliderComponent >() )
        {
            output << YAML::Key << "BoxColliderComponent";
            output << YAML::BeginMap;

            auto &box_collider_component = entity.getComponent< BoxColliderComponent >();
            output << YAML::Key << "Size" << YAML::Value << box_collider_component.size;
            output << YAML::Key << "Offset" << YAML::Value << box_collider_component.offset;
            output << YAML::Key << "bTrigger" << YAML::Value << box_collider_component.trigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << box_collider_component.showColliderBounds;

            /*auto& pPhysicsMaterial = boxColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< SphereColliderComponent >() )
        {
            output << YAML::Key << "SphereColliderComponent";
            output << YAML::BeginMap;

            auto &sphere_collider_component = entity.getComponent< SphereColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << sphere_collider_component.radius;
            output << YAML::Key << "bTrigger" << YAML::Value << sphere_collider_component.trigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << sphere_collider_component.showColliderBounds;

            /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.hasComponent< CapsuleColliderComponent >() )
        {
            output << YAML::Key << "CapsuleColliderComponent";
            output << YAML::BeginMap;

            auto &capsule_collider_component = entity.getComponent< CapsuleColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << capsule_collider_component.radius;
            output << YAML::Key << "Height" << YAML::Value << capsule_collider_component.height;
            output << YAML::Key << "bTrigger" << YAML::Value << capsule_collider_component.trigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value
                   << capsule_collider_component.showColliderBounds;

            output << YAML::EndMap;
        }

        output << YAML::EndMap;
    }

    void SceneSerializer::serializeRuntime( const std::string &file_path )
    {
        SM_ASSERT( false, "SceneSerializer::serializeRuntime > Not implemented" );
    }

    bool SceneSerializer::deserialize( const std::string &file_path )
    {
        std::ifstream file_input{ file_path };
        std::stringstream str_stream{};
        str_stream << file_input.rdbuf();

        YAML::Node data = YAML::Load( str_stream.str() );
        if ( !data["Scene"] )
            return false;

        std::string scene_name = data["Scene"].as< std::string >();
        SM_LOG_TRACE( "Deserializing scene '%s'", scene_name.c_str() );

        auto entities = data["Entities"];
        if ( entities )
        {
            for ( auto entity : entities )
            {
                uint64_t uuid = entity["Entity"].as< Uint64 >();

                std::string name{};
                auto tag_component = entity["TagComponent"];
                if ( tag_component )
                    name = tag_component["Tag"].as< std::string >();

                SM_LOG_TRACE( "Deserialized entity with ID: %llu, name: %s", uuid, name.c_str() );

                Entity deserialized_entity = scene->createEntity( uuid, name );

                auto transform_component = entity["TransformComponent"];
                if ( transform_component )
                {
                    auto &tc = deserialized_entity.getComponent< TransformComponent >();
                    tc.translation = transform_component["Translation"].as< DirectX::XMFLOAT3 >();
                    tc.rotation = transform_component["Rotation"].as< DirectX::XMFLOAT3 >();
                    tc.scale = transform_component["Scale"].as< DirectX::XMFLOAT3 >();
                }

                auto camera_component = entity["CameraComponent"];
                if ( camera_component )
                {
                    auto &cc = deserialized_entity.addComponent< CameraComponent >();

                    auto camera_props = camera_component["Camera"];
                    cc.camera.setProjectionType(
                        static_cast< SceneCamera::ProjectionType >( camera_props["ProjectionType"].as< int >() ) );

                    cc.camera.setFOV( camera_props["FOV"].as< float >() );
                    cc.camera.setPerspectiveNearPlane( camera_props["PerspectiveNearPlane"].as< float >() );
                    cc.camera.setPerspectiveFarPlane( camera_props["PerspectiveFarPlane"].as< float >() );

                    cc.camera.setSize( camera_props["Size"].as< float >() );
                    cc.camera.setOrthographicNearPlane( camera_props["OrthographicNearPlane"].as< float >() );
                    cc.camera.setOrthographicFarPlane( camera_props["OrthographicFarPlane"].as< float >() );

                    cc.primary = camera_component["bPrimary"].as< bool >();
                    cc.fixedAspectRatio = camera_component["bFixedAspectRatio"].as< bool >();
                }

                auto static_mesh_component = entity["StaticMeshComponent"];
                if ( static_mesh_component )
                {
                    auto &smc = deserialized_entity.addComponent< StaticMeshComponent >();

                    const auto &mesh_path = static_mesh_component["Mesh"].as< std::string >();
                    if ( !mesh_path.empty() )
                    {
                        smc.meshes = graphic::MeshLoader::loadStaticMesh( mesh_path );
                        const auto &buffer_layout = smc.materials[0]->getBufferLayout();
                        for ( const auto &mesh : smc.meshes )
                        {
                            mesh->create( buffer_layout );
                        }
                    }

                    auto material = static_mesh_component["Material"];

                    auto float_values = material["FloatValues"];
                    for ( auto it{ float_values.begin() }; it != float_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        smc.materials[0]->setFloatValue( semantic, value );
                    }

                    auto int_values = material["IntValues"];
                    for ( auto it{ int_values.begin() }; it != int_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        smc.materials[0]->setIntValue( semantic, value );
                    }

                    auto bool_values = material["BoolValues"];
                    for ( auto it{ bool_values.begin() }; it != bool_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        smc.materials[0]->setBoolValue( semantic, value );
                    }

                    auto float2_values = material["Float2Values"];
                    for ( auto it{ float2_values.begin() }; it != float2_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        smc.materials[0]->setFloat2Value( semantic, value );
                    }

                    auto float3_values = material["Float3Values"];
                    for ( auto it{ float3_values.begin() }; it != float3_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        smc.materials[0]->setFloat3Value( semantic, value );
                    }

                    auto texture_2d_values = material["Texture2DValues"];
                    for ( auto it{ texture_2d_values.begin() }; it != texture_2d_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto path = ( *it ).second.as< std::string >();
                        if ( !path.empty() )
                            smc.materials[0]->setTexture2D( semantic, graphic::Texture2D::create( path ) );
                    }
                }

                auto skinned_mesh_component = entity["SkinnedMeshComponent"];
                if ( skinned_mesh_component )
                {
                    auto &smc = deserialized_entity.addComponent< SkinnedMeshComponent >();

                    const auto &mesh_path = skinned_mesh_component["Mesh"].as< std::string >();
                    if ( !mesh_path.empty() )
                    {
                        smc.meshes = graphic::MeshLoader::loadSkinnedMesh( mesh_path );
                        const auto &buffer_layout = smc.materials[0]->getBufferLayout();
                        for ( const auto &mesh : smc.meshes )
                        {
                            mesh->create( buffer_layout );

                            if ( mesh->hasAnimations() )
                            {
                                graphic::MeshAnimator animator{ mesh };
                                smc.animators.push_back( animator );
                                smc.animators.back().setAnimation( 0 );
                            }
                        }
                    }

                    auto material = skinned_mesh_component["Material"];

                    auto float_values = material["FloatValues"];
                    for ( auto it{ float_values.begin() }; it != float_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< float >();
                        smc.materials[0]->setFloatValue( semantic, value );
                    }

                    auto int_values = material["IntValues"];
                    for ( auto it{ int_values.begin() }; it != int_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< int >();
                        smc.materials[0]->setIntValue( semantic, value );
                    }

                    auto bool_values = material["BoolValues"];
                    for ( auto it{ bool_values.begin() }; it != bool_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< bool >();
                        smc.materials[0]->setBoolValue( semantic, value );
                    }

                    auto float2_values = material["Float2Values"];
                    for ( auto it{ float2_values.begin() }; it != float2_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT2 >();
                        smc.materials[0]->setFloat2Value( semantic, value );
                    }

                    auto float3_values = material["Float3Values"];
                    for ( auto it{ float3_values.begin() }; it != float3_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto value = ( *it ).second.as< DirectX::XMFLOAT3 >();
                        smc.materials[0]->setFloat3Value( semantic, value );
                    }

                    auto texture_2d_values = material["Texture2DValues"];
                    for ( auto it{ texture_2d_values.begin() }; it != texture_2d_values.end(); ++it )
                    {
                        std::string semantic = ( *it ).first.as< std::string >();
                        auto path = ( *it ).second.as< std::string >();
                        if ( !path.empty() )
                            smc.materials[0]->setTexture2D( semantic, graphic::Texture2D::create( path ) );
                    }
                }

                auto rigidbody_component = entity["RigidbodyComponent"];
                if ( rigidbody_component )
                {
                    auto &rbc = deserialized_entity.addComponent< RigidbodyComponent >();

                    rbc.bodyType =
                        static_cast< RigidbodyComponent::BodyType >( rigidbody_component["BodyType"].as< int >() );
                    rbc.collisionDetectionType = static_cast< RigidbodyComponent::CollisionDetectionType >(
                        rigidbody_component["CollisionDetectionType"].as< int >() );

                    auto physics_material = rigidbody_component["PhysicsMaterial"];
                    /*rbc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    rbc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    rbc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

                    rbc.mass = rigidbody_component["Mass"].as< float >();
                    rbc.linearDrag = rigidbody_component["LinearDrag"].as< float >();
                    rbc.angularDrag = rigidbody_component["AngularDrag"].as< float >();

                    rbc.disableGravity = rigidbody_component["bDisableGravity"].as< bool >();
                    rbc.kinematic = rigidbody_component["bKinematic"].as< bool >();

                    rbc.lockPositionX = rigidbody_component["bLockPositionX"].as< bool >();
                    rbc.lockPositionY = rigidbody_component["bLockPositionY"].as< bool >();
                    rbc.lockPositionZ = rigidbody_component["bLockPositionZ"].as< bool >();

                    rbc.lockRotationX = rigidbody_component["bLockRotationX"].as< bool >();
                    rbc.lockRotationY = rigidbody_component["bLockRotationY"].as< bool >();
                    rbc.lockRotationZ = rigidbody_component["bLockRotationZ"].as< bool >();
                }

                auto box_collider_component = entity["BoxColliderComponent"];
                if ( box_collider_component )
                {
                    auto &bcc = deserialized_entity.addComponent< BoxColliderComponent >();

                    bcc.size = box_collider_component["Size"].as< DirectX::XMFLOAT3 >();
                    bcc.offset = box_collider_component["Offset"].as< DirectX::XMFLOAT3 >();
                    bcc.trigger = box_collider_component["bTrigger"].as< bool >();
                    bcc.showColliderBounds = box_collider_component["bShowColliderBounds"].as< bool >();

                    /*auto physicsMaterial = rigidBodyComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto sphere_collider_component = entity["SphereColliderComponent"];
                if ( sphere_collider_component )
                {
                    auto &scc = deserialized_entity.addComponent< SphereColliderComponent >();

                    scc.radius = sphere_collider_component["Radius"].as< float >();
                    scc.trigger = sphere_collider_component["bTrigger"].as< bool >();
                    scc.showColliderBounds = sphere_collider_component["bShowColliderBounds"].as< bool >();

                    /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
                    bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
                    bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
                    bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
                }

                auto capsule_collider_component = entity["CapsuleColliderComponent"];
                if ( capsule_collider_component )
                {
                    auto &ccc = deserialized_entity.addComponent< CapsuleColliderComponent >();

                    ccc.radius = capsule_collider_component["Radius"].as< float >();
                    ccc.height = capsule_collider_component["Height"].as< float >();
                    ccc.trigger = capsule_collider_component["bTrigger"].as< bool >();
                    ccc.showColliderBounds = capsule_collider_component["bShowColliderBounds"].as< bool >();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::deserializeRuntime( const std::string &file_path )
    {
        SM_ASSERT( false, "SceneSerializer::deserializeRuntime > Not implemented" );
        return false;
    }
}