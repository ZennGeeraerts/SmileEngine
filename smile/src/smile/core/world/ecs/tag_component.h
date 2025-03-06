/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include <string>

namespace smile::world::ecs
{
    struct TagComponent
    {
        TagComponent() = default;
        TagComponent( const TagComponent & ) = default;
        TagComponent( const std::string &tag ) : Tag{ tag }
        {
        }

        std::string Tag;
    };
}