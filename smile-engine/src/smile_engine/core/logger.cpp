#include "smpch.h"
#include "logger.h"

namespace smile
{
    LogPriority Logger::priority = LogPriority::Trace;
    std::mutex Logger::mutex{};

    void Logger::setPriority( LogPriority logPriority )
    {
        priority = logPriority;
    }
}