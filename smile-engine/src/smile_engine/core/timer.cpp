#include "smpch.h"
#include "timer.h"

using namespace std::chrono;

namespace Smile
{
    Timer::Timer()
        : m_MsPerFrame{ 16 },
          m_Lag{ 0.0f },
          m_TotalTimePassed{ 0.0f },
          m_DeltaTime{ 0.0f },
          m_FPS{ 0 },
          m_LastTime{},
          m_IsRunning{ false }
    {
    }

    void Timer::Run()
    {
        m_IsRunning = true;
        m_LastTime = high_resolution_clock::now();
    }

    void Timer::OnUpdate()
    {
        if ( !m_IsRunning )
            return;

        const auto currentTime{ high_resolution_clock::now() };
        m_DeltaTime = duration< float >( currentTime - m_LastTime ).count();
        m_LastTime = currentTime;

        m_Lag += m_DeltaTime;

        m_FPS = static_cast< Uint32 >( 1.f / m_DeltaTime.GetSeconds() );
    }

    bool Timer::IsCatchingUpInFixedSteps()
    {
        if ( m_Lag >= m_MsPerFrame )
        {
            m_Lag -= static_cast< float >( m_MsPerFrame );
            return true;
        }

        return false;
    }
}
