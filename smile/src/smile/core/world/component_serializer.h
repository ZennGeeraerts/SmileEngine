/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/yaml/yaml.h"

namespace smile::world
{
    class Entity;

    class ComponentSerializerBase
    {
      public:
        virtual ~ComponentSerializerBase() = default;

        virtual bool TrySerialize( yaml::Emitter &output, Entity entity ) = 0;
        virtual bool TryDeserialize( const yaml::Node &data, Entity entity ) = 0;
    };

    template < typename ComponentType >
    class ComponentSerializer final : public ComponentSerializerBase
    {
      public:
        bool TrySerialize( yaml::Emitter &output, Entity entity ) override;
        bool TryDeserialize( const yaml::Node &data, Entity entity ) override;
    };
}