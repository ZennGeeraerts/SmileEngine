#include "smpch.h"
#include "logger.h"

namespace Smile
{
    LogPriority Logger::m_Priority = LogPriority::Trace;
    std::mutex Logger::m_Mutex{};

    void Logger::SetPriority( LogPriority logPriority )
    {
        m_Priority = logPriority;
    }
}