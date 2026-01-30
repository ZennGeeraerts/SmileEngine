/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "logger_registry.h"

#include "sink/stdout_color_sink.h"

namespace smile::logging
{
    LoggerRegistry::LoggerRegistry()
    {
        primitive::String defaultLoggerName = "";
        m_pDefaultLogger = CreateRef< Logger >( defaultLoggerName );
        auto pStdoutColorSink = CreateRef< StdoutColorSink >();
        m_pDefaultLogger->AddSink( pStdoutColorSink );

        m_LoggerMap[defaultLoggerName] = m_pDefaultLogger;
    }
}