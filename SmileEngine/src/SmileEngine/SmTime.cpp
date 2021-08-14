#include "smpch.h"
#include "SmTime.h"

using namespace std::chrono;

namespace Smile
{
	SmTime::SmTime()
		: m_MsPerFrame{ 16 }
		, m_Lag{ 0.0f }
		, m_TotalTimePassed{ 0.0f }
		, m_DeltaTime{ 0.0f }
		, m_FPS{ 0 }
		, m_LastTime{}
		, m_bRunning{ false }
	{

	}

	void SmTime::Run()
	{
		m_bRunning = true;
		m_LastTime = high_resolution_clock::now();
	}

	void SmTime::OnUpdate()
	{
		if (!m_bRunning)
		{
			return;
		}

		const auto currentTime{ high_resolution_clock::now() };
		m_DeltaTime = duration<float>(currentTime - m_LastTime).count();
		m_LastTime = currentTime;
		m_Lag += m_DeltaTime;

		m_FPS = int(1.f / m_DeltaTime);
	}

	bool SmTime::IsCatchingUpInFixedSteps()
	{
		if (m_Lag >= m_MsPerFrame)
		{
			m_Lag -= m_MsPerFrame;
			return true;
		}

		return false;
	}

	float SmTime::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	float SmTime::GetTotalTimePassed() const
	{
		return m_TotalTimePassed;
	}

	uint32_t SmTime::GetFPS() const
	{
		return m_FPS;
	}

	void SmTime::SetMsPerFrame(uint32_t msPerFrame)
	{
		m_MsPerFrame = msPerFrame;
	}

	uint32_t SmTime::GetMsPerFrame() const
	{
		return m_MsPerFrame;
	}

	std::chrono::steady_clock::time_point SmTime::GetTimeBeforeGameLoop() const
	{
		return m_LastTime;
	}

	bool SmTime::Running() const
	{
		return m_bRunning;
	}

	void SmTime::Stop()
	{
		m_bRunning = false;
	}
}
