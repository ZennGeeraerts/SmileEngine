#include "smpch.h"
#include "Timer.h"

using namespace std::chrono;

namespace smile
{
    Timer::Timer()
        : m_MsPerFrame{ 16 },
          m_Lag{ 0.0f },
          m_TotalTimePassed{ 0.0f },
          m_DeltaTime{ 0.0f },
          m_FPS{ 0 },
          m_LastTime{},
          m_bRunning{ false }
    {
    }

    void Timer::Run()
    {
        m_bRunning = true;
        m_LastTime = high_resolution_clock::now();
    }

    void Timer::OnUpdate()
    {
        if ( !m_bRunning )
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

    Timestep Timer::GetDeltaTime() const
    {
        return m_DeltaTime;
    }

    Timestep Timer::GetTotalTimePassed() const
    {
        return m_TotalTimePassed;
    }

    uint32_t Timer::GetFPS() const
    {
        return m_FPS;
    }

    void Timer::SetMsPerFrame( uint32_t msPerFrame )
    {
        m_MsPerFrame = msPerFrame;
    }

    uint32_t Timer::GetMsPerFrame() const
    {
        return m_MsPerFrame;
    }

    std::chrono::steady_clock::time_point Timer::GetTimeBeforeGameLoop() const
    {
        return m_LastTime;
    }
}
