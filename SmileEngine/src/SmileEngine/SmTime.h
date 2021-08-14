#pragma once
#include <chrono>

namespace Smile
{
	class SmTime final
	{
	public:
		inline static SmTime& GetInstance()
		{
			static SmTime instance{};
			return instance;
		}

		void Run();
		void OnUpdate();
		bool IsCatchingUpInFixedSteps();

		float GetDeltaTime() const;
		float GetTotalTimePassed() const;
		uint32_t GetFPS() const;
		void SetMsPerFrame(uint32_t msPerFrame);
		uint32_t GetMsPerFrame() const;
		std::chrono::steady_clock::time_point GetTimeBeforeGameLoop() const;
		bool Running() const;
		void Stop();

	private:
		SmTime();

		int m_MsPerFrame; /* amount of milliseconds per frame for the fixed update */
		float m_Lag;
		float m_TotalTimePassed;
		float m_DeltaTime;
		int m_FPS;
		std::chrono::steady_clock::time_point m_LastTime;
		bool m_bRunning;
	};
}

