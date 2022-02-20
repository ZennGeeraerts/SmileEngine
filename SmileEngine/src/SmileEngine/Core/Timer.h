#pragma once

#include "SmileEngine/Core/Core.h"
#include <chrono>
#include "Timestep.h"

namespace smile
{
    class Timer final
    {
      public:
        inline static Timer &GetInstance()
        {
            static Timer instance{};
            return instance;
        }

        void Run();
        void OnUpdate();
        bool IsCatchingUpInFixedSteps();

        Timestep GetDeltaTime() const;
        Timestep GetTotalTimePassed() const;
        uint32_t GetFPS() const;
        void SetMsPerFrame( uint32_t msPerFrame );
        uint32_t GetMsPerFrame() const;
        std::chrono::steady_clock::time_point GetTimeBeforeGameLoop() const;

      private:
        Timer();

        uint32_t m_MsPerFrame; /* amount of milliseconds per frame for the fixed update */
        Timestep m_Lag;
        Timestep m_TotalTimePassed;
        Timestep m_DeltaTime;
        uint32_t m_FPS;
        std::chrono::steady_clock::time_point m_LastTime;
        bool m_bRunning;
    };
}
