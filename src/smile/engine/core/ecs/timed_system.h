/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "system.h"

#include "engine/core/application/timer.h"

namespace smile::ecs
{
    template < typename DerivedType >
    class TimedSystem : public System< DerivedType >
    {
      public:
        TimedSystem() = default;
        virtual ~TimedSystem() = default;

        void OnUpdate() override
        {
            const auto &timer = smile::application::Timer::GetInstance();
            static_cast< DerivedType * >( this )->OnUpdate( timer.GetDeltaTime() );
        }
    };
}