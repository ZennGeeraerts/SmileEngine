/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/common/primitive/uuid.h"

namespace smile::world::ecs
{
    struct IDComponent
    {
        IDComponent() = default;
        IDComponent( const IDComponent & ) = default;
        IDComponent( primitive::UUID uuid ) : ID{ uuid }
        {
        }

        primitive::UUID ID;
    };
}