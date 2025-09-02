/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "serializer.h"

#include "smile/common/foundation/weak_function.h"
#include "smile/core/yaml/string.h"
#include "smile/core/yaml/math.h"

SM_FOUNDATION_WEAK_FUNCTION_DECLARE(
    void SerializeAdditional( smile::yaml::Emitter &output, smile::world::Entity entity ) );

SM_FOUNDATION_WEAK_FUNCTION_DECLARE(
    void DeserializeAdditional( const smile::yaml::Node &data, smile::world::Entity entity ) );

SM_FOUNDATION_WEAK_FUNCTION_IMPLEMENT( void, SerializeAdditional, smile::yaml::Emitter &, smile::world::Entity )
{
}

SM_FOUNDATION_WEAK_FUNCTION_IMPLEMENT( void, DeserializeAdditional, const smile::yaml::Node &, smile::world::Entity )
{
}

namespace smile::serializer
{
    void Serialize( yaml::Emitter &output, world::Entity entity )
    {
        SM_ASSERT_MSG( entity.HasComponent< world::ecs::IDComponent >(),
            "WorldSerializer::SerializeWorld > Entity does not have an IDComponent" );

        output << YAML::BeginMap;
        output << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        if ( entity.HasComponent< world::ecs::TagComponent >() )
        {
            output << YAML::Key << "TagComponent";
            output << YAML::BeginMap;

            auto &tag = entity.GetComponent< world::ecs::TagComponent >().Tag;
            output << YAML::Key << "Tag" << YAML::Value << tag;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< world::ecs::TransformComponent >() )
        {
            output << YAML::Key << "TransformComponent";
            output << YAML::BeginMap;

            auto &transformComponent = entity.GetComponent< world::ecs::TransformComponent >();
            output << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
            output << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
            output << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

            output << YAML::EndMap;
        }

        using SerializeAdditionalFunction = void ( * )( yaml::Emitter &, world::Entity );
        SerializeAdditionalFunction serializeAdditional;

        SM_FOUNDATION_WEAK_FUNCTION_RESOLVE( SerializeAdditionalFunction, serializeAdditional, SerializeAdditional );

        ( *serializeAdditional )( output, entity );

        output << YAML::EndMap;
    }

    void Deserialize( const yaml::Node &data, world::Entity entity )
    {
        auto transformData = data["TransformComponent"];
        if ( transformData )
        {
            auto &transformComponent = entity.GetComponent< world::ecs::TransformComponent >();
            transformComponent.Translation = transformData["Translation"].as< DirectX::XMFLOAT3 >();
            transformComponent.Rotation = transformData["Rotation"].as< DirectX::XMFLOAT3 >();
            transformComponent.Scale = transformData["Scale"].as< DirectX::XMFLOAT3 >();
        }

        using DserializeAdditionalFunction = void ( * )( const yaml::Node &, world::Entity );
        DserializeAdditionalFunction deserializeAdditional;

        SM_FOUNDATION_WEAK_FUNCTION_RESOLVE(
            DserializeAdditionalFunction, deserializeAdditional, DeserializeAdditional );

        ( *deserializeAdditional )( data, entity );
    }
}