#include "smpch.h"
#include "logger.h"

namespace smile
{
    LogPriority Logger::s_Priority = LogPriority::TRACE;
    std::mutex Logger::s_Mutex{};

    void Logger::SetPriority( LogPriority logPriority )
    {
        s_Priority = logPriority;
    }
}