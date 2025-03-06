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

        primitive::UUID ID;
    };
}