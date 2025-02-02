/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "log_message.h"

namespace smile::logging
{
    LogMessage::LogMessage( std::string_view loggerName,
        LogLevel level,
        std::string_view message,
        LogSource source,
        std::chrono::system_clock::time_point time )
        : LoggerName{ loggerName }, Level{ level }, Time{ time }, Source{ source }, Payload{ message }
    {
    }

    LogMessage::LogMessage( std::string_view loggerName, LogLevel level, std::string_view message, LogSource source )
        : LogMessage{ loggerName, level, message, source, std::chrono::system_clock::now() }
    {
    }
}