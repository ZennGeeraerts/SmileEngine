/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "log_sink.h"

#include "logging/console_mutex.h"
#include "logging/format/pattern_formatter.h"

namespace smile::logging
{
    LogSink::LogSink()
        : BaseLogger{ LogLevel::Trace },
          m_Mutex{ ConsoleMutex::GetMutex() },
          m_pFormatter{ CreateScope< PatternFormatter >() }
    {
    }

    void LogSink::SetFormatter( Scope< Formatter > pFormatter )
    {
        std::lock_guard< std::mutex > lock{ m_Mutex };
        m_pFormatter = std::move( pFormatter );
    }
}