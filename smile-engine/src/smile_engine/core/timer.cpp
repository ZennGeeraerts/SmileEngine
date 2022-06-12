#include "smpch.h"
#include "timer.h"

using namespace std::chrono;

namespace smile
{
    Timer::Timer()
        : msPerFrame{ 16 },
          lag{ 0.0f },
          totalTimePassed{ 0.0f },
          deltaTime{ 0.0f },
          fps{ 0 },
          lastTime{},
          isRunning{ false }
    {
    }

    void Timer::run()
    {
        isRunning = true;
        lastTime = high_resolution_clock::now();
    }

    void Timer::onUpdate()
    {
        if ( !isRunning )
            return;

        const auto current_time{ high_resolution_clock::now() };
        deltaTime = duration< float >( current_time - lastTime ).count();
        lastTime = current_time;

        lag += deltaTime;

        fps = static_cast< Uint32 >( 1.f / deltaTime.getSeconds() );
    }

    bool Timer::isCatchingUpInFixedSteps()
    {
        if ( lag >= msPerFrame )
        {
            lag -= static_cast< float >( msPerFrame );
            return true;
        }

        return false;
    }
}
