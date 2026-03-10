/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world_serializer.h"

#include "smile/common/logging/logger.h"
#include "entity.h"
#include "smile/core/yaml/string.h"
#include "smile/core/serializer/serializer.h"

#include <fstream>

namespace smile::world
{
    WorldSerializer::WorldSerializer( memory::Ref< World > pWorld ) : m_pWorld{ pWorld }
    {
    }

    void WorldSerializer::Serialize( const std::filesystem::path &filePath )
    {
        YAML::Emitter output{};
        output << YAML::BeginMap;
        output << YAML::Key << "World" << YAML::Value << "Untitled";
        output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

#ifdef SM_WITH_SERIALIZER
        m_pWorld->m_ECSEngine.Each(
            [&]( auto entityID )
            {
                Entity entity{ entityID, m_pWorld.GetPointer() };
                if ( !entity )
                    return;

                serializer::Serialize( output, entity );
            } );
#endif

        output << YAML::EndSeq;
        output << YAML::EndMap;

        std::ofstream fileOutput{ filePath };
        fileOutput << output.c_str();
    }

    void WorldSerializer::SerializeRuntime( const std::filesystem::path &filePath )
    {
        SM_ASSERT_MSG( false, "WorldSerializer::SerializeRuntime > Not implemented" );
    }

    bool WorldSerializer::Deserialize( const std::filesystem::path &filePath )
    {
        YAML::Node data;
        try
        {
            data = YAML::LoadFile( filePath.string() );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_ERROR( "Failed to load .smile file: {0}\n{1}", filePath.string(), e.what() );
            return false;
        }

        if ( !data["World"] )
            return false;

        primitive::String worldName = data["World"].as< primitive::String >();
        SM_LOG_TRACE( "Deserializing world '{}'", worldName );

        auto entities = data["Entities"];
        if ( entities )
        {
            for ( auto entity : entities )
            {
                Uint64 uuid = entity["Entity"].as< Uint64 >();

                primitive::String name{};
                auto tagComponent = entity["TagComponent"];
                if ( tagComponent )
                    name = tagComponent["Tag"].as< primitive::String >();

                SM_LOG_TRACE( "Deserialized entity with ID: {0}, name: {1}", uuid, name );

                Entity deserializedEntity = m_pWorld->CreateEntity( uuid, name );

#ifdef SM_WITH_SERIALIZER
                serializer::Deserialize( entity, deserializedEntity );
#endif
            }
        }

        return true;
    }

    bool WorldSerializer::DeserializeRuntime( const std::filesystem::path &filePath )
    {
        SM_ASSERT_MSG( false, "WorldSerializer::DeserializeRuntime > Not implemented" );
        return false;
    }
}