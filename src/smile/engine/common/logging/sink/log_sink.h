/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "logging/base_logger.h"
#include "logging/log_message.h"

namespace smile::logging
{
    class LogSink : public BaseLogger
    {
      public:
        LogSink() : BaseLogger{ LogLevel::Trace }
        {
        }
        virtual ~LogSink() = default;

        virtual void Log( const LogMessage &message ) = 0;
    };
}