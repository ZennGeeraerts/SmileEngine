#include "smpch.h"
#include "Logger.h"

namespace Smile
{
	LogPriority Logger::m_Priority = LogPriority::eTrace;
	std::mutex Logger::m_Mutex{};

	void Logger::SetPriority(LogPriority logPriority)
	{
		m_Priority = logPriority;
	}
}