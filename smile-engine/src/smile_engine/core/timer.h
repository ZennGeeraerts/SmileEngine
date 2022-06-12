#pragma once

#include "smile_engine/core/core.h"
#include "timestep.h"

#include <chrono>

namespace smile
{
    class Timer final
    {
      public:
        inline static Timer &getInstance()
        {
            static Timer instance{};
            return instance;
        }

        void run();
        void onUpdate();
        bool isCatchingUpInFixedSteps();

        Timestep getDeltaTime() const
        {
            return deltaTime;
        }
        Timestep getTotalTimePassed() const
        {
            return totalTimePassed;
        }
        Uint32 getFPS() const
        {
            return fps;
        }
        void setMsPerFrame( Uint32 ms_per_frame )
        {
            msPerFrame = ms_per_frame;
        }
        Uint32 getMsPerFrame() const
        {
            return msPerFrame;
        }
        std::chrono::steady_clock::time_point getTimeBeforeGameLoop() const
        {
            return lastTime;
        }

      private:
        Timer();

      private:
        Uint32 msPerFrame; /* amount of milliseconds per frame for the fixed update */
        Timestep lag;
        Timestep totalTimePassed;
        Timestep deltaTime;
        Uint32 fps;
        std::chrono::steady_clock::time_point lastTime;
        bool isRunning;
    };
}
