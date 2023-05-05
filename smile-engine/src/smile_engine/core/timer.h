#pragma once

#include "smile_engine/core/core.h"
#include "timestep.h"
#include "singleton.h"

#include <chrono>

namespace smile
{
    class Timer final : public Singleton< Timer >
    {
      public:
        Timer();
        void Run();
        void OnUpdate();
        bool IsCatchingUpInFixedSteps();

        Timestep GetDeltaTime() const
        {
            return m_DeltaTime;
        }
        Timestep GetTotalTimePassed() const
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
        Timestep m_Lag;
        Timestep m_TotalTimePassed;
        Timestep m_DeltaTime;
        Uint32 m_FPS;
        std::chrono::steady_clock::time_point m_LastTime;
        bool m_IsRunning;
    };
}
