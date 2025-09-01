/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "serializer.h"

#include "smile/core/yaml/string.h"
#include "smile/scripting/ecs/script_component.h"

namespace smile::scripting::serializer
{
    void Serialize( yaml::Emitter &output, world::Entity entity )
    {
        if ( entity.HasComponent< ecs::ScriptComponent >() )
        {
            output << YAML::Key << "ScriptComponent";
            output << YAML::BeginMap;

            auto &scriptComponent = entity.GetComponent< ecs::ScriptComponent >();
            output << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

            output << YAML::EndMap;
        }
    }

    void Deserialize( const yaml::Node &data, world::Entity entity )
    {
        auto scriptComponent = data["ScriptComponent"];
        if ( scriptComponent )
        {
            auto &sc = entity.AddComponent< ecs::ScriptComponent >();
            sc.ClassName = scriptComponent["ClassName"].as< std::string >();
        }
    }
}