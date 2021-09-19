#pragma once
#include "Core.h"
#include <mutex>

#define SM_NRM  "\x1B[0m"
#define SM_RED  "\x1B[31m"
#define SM_GRN  "\x1B[32m"
#define SM_YEL  "\x1B[33m"
#define SM_BLU  "\x1B[34m"
#define SM_MAG  "\x1B[35m"
#define SM_CYN  "\x1B[36m"
#define SM_WHT  "\x1B[37m"

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

	class Logger final
	{
	public:
		static void SetPriority(LogPriority logPriority);

		template<typename... Args>
		static void LogTrace(const char* message, Args... args)
		{
			Log(LogPriority::eTrace, "Trace", SM_NRM, message, args...);
		}

		template<typename... Args>
		static void LogDebug(const char* message, Args... args)
		{
			Log(LogPriority::eDebug, "Debug", SM_CYN, message, args...);
		}

		template<typename... Args>
		static void LogInfo(const char* message, Args... args)
		{
			Log(LogPriority::eInfo, "Info", SM_GRN, message, args...);
		}

		template<typename... Args>
		static void LogWarning(const char* message, Args... args)
		{
			Log(LogPriority::eWarning, "Warning", SM_YEL, message, args...);
		}

		template<typename... Args>
		static void LogError(const char* message, Args... args)
		{
			Log(LogPriority::eError, "Error", SM_MAG, message, args...);
		}

		template<typename... Args>
		static void LogCriticalError(const char* message, Args... args)
		{
			Log(LogPriority::eCriticalError, "CriticalError", SM_RED, message, args...);
		}

	private:
		template<typename... Args>
		static void Log(LogPriority logPriority, const char* logName, const char* color , const char* message, Args... args)
		{
			if (m_Priority <= logPriority)
			{
				std::lock_guard<std::mutex> lock{ m_Mutex };
				printf("%s", color);
				printf("[%s]\t", logName);
				printf(message, args...);
				printf("\n");
				printf("%s", SM_NRM);
			}
		}

		static LogPriority m_Priority;
		static std::mutex m_Mutex;
	};
}

// Macro's are used for the log functions, so the functionality can easily be removed if we want a distribution build
#define SM_LOG_TRACE(...)			::Smile::Logger::LogTrace(__VA_ARGS__)
#define SM_LOG_DEBUG(...)			::Smile::Logger::LogDebug(__VA_ARGS__)
#define SM_LOG_INFO(...)			::Smile::Logger::LogInfo(__VA_ARGS__)
#define SM_LOG_WARNING(...)			::Smile::Logger::LogWarning(__VA_ARGS__)
#define SM_LOG_ERROR(...)			::Smile::Logger::LogError(__VA_ARGS__)
#define SM_LOG_CRITICALERROR(...)	::Smile::Logger::LogCriticalError(__VA_ARGS__)