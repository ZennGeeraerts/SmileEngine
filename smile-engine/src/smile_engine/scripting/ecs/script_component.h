/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include <string>

namespace smile::scripting::ecs
{
    struct ScriptComponent final
    {
        ScriptComponent() = default;
        ScriptComponent( const ScriptComponent & ) = default;

        std::string ClassName;
    };
}