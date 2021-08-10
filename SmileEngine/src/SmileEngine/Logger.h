#pragma once
#include "Core.h"
#include <mutex>

namespace Smile
{
	enum class LogPriority
	{
		eTrace,
		eDebug,
		eInfo,
		eWarning,
		eError,
		eCriticalError
	};

	class SMILE_API Logger final
	{
	public:
		static void SetPriority(LogPriority logPriority);

		template<typename... Args>
		static void LogTrace(const char* message, Args... args)
		{
			Log(LogPriority::eTrace, "Trace", message, args...);
		}

		template<typename... Args>
		static void LogDebug(const char* message, Args... args)
		{
			Log(LogPriority::eDebug, "Debug", message, args...);
		}

		template<typename... Args>
		static void LogInfo(const char* message, Args... args)
		{
			Log(LogPriority::eInfo, "Info", message, args...);
		}

		template<typename... Args>
		static void LogWarning(const char* message, Args... args)
		{
			Log(LogPriority::eWarning, "Warning", message, args...);
		}

		template<typename... Args>
		static void LogError(const char* message, Args... args)
		{
			Log(LogPriority::eError, "Error", message, args...);
		}

		template<typename... Args>
		static void LogCriticalError(const char* message, Args... args)
		{
			Log(LogPriority::eCriticalError, "CriticalError", message, args...);
		}

	private:
		template<typename... Args>
		static void Log(LogPriority logPriority, const char* logName, const char* message, Args... args)
		{
			if (m_Priority <= logPriority)
			{
				std::lock_guard<std::mutex> lock{ m_Mutex };
				printf("[%s]\t", logName);
				printf(message, args...);
				printf("\n");
			}
		}

		static LogPriority m_Priority;
		static std::mutex m_Mutex;
	};
}

#define SM_TRACE(...)			::Smile::Logger::LogTrace(__VA_ARGS__)
#define SM_DEBUG(...)			::Smile::Logger::LogDebug(__VA_ARGS__)
#define SM_INFO(...)			::Smile::Logger::LogInfo(__VA_ARGS__)
#define SM_WARNING(...)			::Smile::Logger::LogWarning(__VA_ARGS__)
#define SM_ERROR(...)			::Smile::Logger::LogError(__VA_ARGS__)
#define SM_CRITICALERROR(...)	::Smile::Logger::CriticalError(__VA_ARGS__)
