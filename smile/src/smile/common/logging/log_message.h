/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "log_level.h"
#include "log_source.h"
#include "smile/common/primitive/text/string_view.h"

#include <chrono>

namespace smile::logging
{
    struct LogMessage final
    {
        LogMessage( primitive::StringView loggerName,
            LogLevel level,
            primitive::StringView message,
            LogSource source,
            std::chrono::system_clock::time_point time );

        LogMessage( primitive::StringView loggerName, LogLevel level, primitive::StringView message, LogSource source );

        primitive::StringView LoggerName;
        LogLevel Level;
        primitive::StringView Payload;
        LogSource Source;
        std::chrono::system_clock::time_point Time;
    };
}