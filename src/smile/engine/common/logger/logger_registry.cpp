/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "logger_registry.h"

#include "stdout_sink.h"

namespace smile::logging
{
    LoggerRegistry::LoggerRegistry()
    {
        std::string defaultLoggerName = "";
        m_pDefaultLogger = CreateRef< Logger >( defaultLoggerName );
        auto pStdoutSink = CreateRef< StdoutSink >();
        m_pDefaultLogger->AddSink( std::move( pStdoutSink ) );

        m_LoggerMap[defaultLoggerName] = m_pDefaultLogger;
    }
}