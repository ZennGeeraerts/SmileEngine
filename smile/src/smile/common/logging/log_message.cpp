/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "log_message.h"

namespace smile::logging
{
    LogMessage::LogMessage( primitive::StringView loggerName,
        LogLevel level,
        primitive::StringView message,
        LogSource source,
        std::chrono::system_clock::time_point time )
        : LoggerName{ loggerName }, Level{ level }, Time{ time }, Source{ source }, Payload{ message }
    {
    }

    LogMessage::LogMessage( primitive::StringView loggerName,
        LogLevel level,
        primitive::StringView message,
        LogSource source )
        : LogMessage{ loggerName, level, message, source, std::chrono::system_clock::now() }
    {
    }
}