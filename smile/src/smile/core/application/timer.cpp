/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "timer.h"

#include "smile/common/platform/clock.h"

namespace smile::application
{
    void Timer::Run() noexcept
    {
        m_IsRunning = true;
        m_LastTime = platform::Clock::GetTime();
    }

    void Timer::OnUpdate() noexcept
    {
        if ( !m_IsRunning )
            return;

        const auto currentTime{ platform::Clock::GetTime() };
        m_DeltaTime = ( currentTime - m_LastTime ) * 1_us;
        m_LastTime = currentTime;

        m_Lag += m_DeltaTime;

        m_FPS = static_cast< Uint32 >( 1.f / m_DeltaTime );

        ++m_Ticks;
    }

    bool Timer::IsCatchingUpInFixedSteps() noexcept
    {
        if ( m_Lag >= m_TimePerFrame )
        {
            m_Lag -= m_TimePerFrame;

            ++m_FixedTicks;

            return true;
        }

        return false;
    }
}
