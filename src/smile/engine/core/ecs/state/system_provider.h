/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::ecs::state
{
    class SystemProvider
    {
      public:
        SystemProvider() = default;
        virtual ~SystemProvider() = default;

        virtual Ref< smile::ecs::BaseSystem > Create() = 0;
    };
}