/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/meyers_singleton.h"
#include "smile/common/platform/system_time.h"
#include "smile/common/primitive/timestep.h"

namespace smile::application
{
    class Timer final : public foundation::MeyersSingleton< Timer >
    {
      public:
        constexpr Timer() noexcept
            : m_TimePerFrame{ 1.0f / 60.0f },
              m_Lag{ 0.0f },
              m_TotalTimePassed{ 0.0f },
              m_DeltaTime{ 0.0f },
              m_FPS{ 0 },
              m_LastTime{},
              m_IsRunning{ false }
        {
        }

        void Run() noexcept;
        void OnUpdate() noexcept;
        bool IsCatchingUpInFixedSteps() noexcept;

        primitive::Timestep GetDeltaTime() const noexcept
        {
            return m_DeltaTime;
        }

        primitive::Timestep GetTotalTimePassed() const noexcept
        {
            return m_TotalTimePassed;
        }

        Uint32 GetFPS() const noexcept
        {
            return m_FPS;
        }

        void SetTimePerFrame( primitive::Timestep timePerFrame ) noexcept
        {
            m_TimePerFrame = timePerFrame;
        }

        primitive::Timestep GetTimePerFrame() const noexcept
        {
            return m_TimePerFrame;
        }

        platform::SystemTime GetTimeBeforeGameLoop() const noexcept
        {
            return m_LastTime;
        }

        Uint64 GetTicks() const noexcept
        {
            return m_Ticks;
        }

        Uint64 GetFixedTicks() const noexcept
        {
            return m_FixedTicks;
        }

      private:
        primitive::Timestep m_TimePerFrame; /* amount of seconds per frame for the fixed update */
        primitive::Timestep m_Lag;
        primitive::Timestep m_TotalTimePassed;
        primitive::Timestep m_DeltaTime;
        Uint32 m_FPS;
        platform::SystemTime m_LastTime;
        bool m_IsRunning;
        Uint64 m_Ticks{ 0 };
        Uint64 m_FixedTicks{ 0 };
    };
}
