/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/singleton.h"
#include "smile/common/primitive/timestep.h"

#include <chrono>

namespace smile::application
{
    class Timer final : public foundation::Singleton< Timer >
    {
      public:
        Timer();
        void Run();
        void OnUpdate();
        bool IsCatchingUpInFixedSteps();

        primitive::Timestep GetDeltaTime() const
        {
            return m_DeltaTime;
        }
        primitive::Timestep GetTotalTimePassed() const
        {
            return m_TotalTimePassed;
        }
        Uint32 GetFPS() const
        {
            return m_FPS;
        }
        void SetMsPerFrame( Uint32 msPerFrame )
        {
            m_MsPerFrame = msPerFrame;
        }
        Uint32 GetMsPerFrame() const
        {
            return m_MsPerFrame;
        }
        std::chrono::steady_clock::time_point GetTimeBeforeGameLoop() const
        {
            return m_LastTime;
        }

      private:
        Uint32 m_MsPerFrame; /* amount of milliseconds per frame for the fixed update */
        primitive::Timestep m_Lag;
        primitive::Timestep m_TotalTimePassed;
        primitive::Timestep m_DeltaTime;
        Uint32 m_FPS;
        std::chrono::steady_clock::time_point m_LastTime;
        bool m_IsRunning;
    };
}
