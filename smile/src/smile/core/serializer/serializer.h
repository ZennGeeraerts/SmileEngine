/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/yaml/yaml.h"
#include "smile/core/world/entity.h"

namespace smile::serializer
{
    void Serialize( yaml::Emitter &output, world::Entity entity );
    void Deserialize( const yaml::Node &data, world::Entity entity );
}