/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "log_level.h"
#include "log_source.h"

#include <string_view>
#include <chrono>

namespace smile::logging
{
    struct LogMessage final
    {
        LogMessage( std::string_view loggerName,
            LogLevel level,
            std::string_view message,
            LogSource source,
            std::chrono::system_clock::time_point time );

        LogMessage( std::string_view loggerName, LogLevel level, std::string_view message, LogSource source );

        std::string_view LoggerName;
        LogLevel Level;
        std::string_view Payload;
        LogSource Source;
        std::chrono::system_clock::time_point Time;
    };
}