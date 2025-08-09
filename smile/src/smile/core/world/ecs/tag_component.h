/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string.h"

namespace smile::world::ecs
{
    struct TagComponent
    {
        TagComponent() = default;
        TagComponent( const TagComponent & ) = default;
        TagComponent( const primitive::String &tag ) : Tag{ tag }
        {
        }

        primitive::String Tag;
    };
}