/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/common/primitive/uuid.h"

namespace smile::scene::ecs
{
    struct IDComponent
    {
        IDComponent() = default;
        IDComponent( const IDComponent & ) = default;

        primitive::UUID ID;
    };
}