/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "stdout_sink.h"

#include "logging/format/pattern_formatter.h"
#include "platform/print.h"

namespace smile::logging
{
    StdoutSink::StdoutSink() : m_Mutex{ ConsoleMutex::GetMutex() }, m_pFormatter{ CreateScope< PatternFormatter >() }
    {
    }

    void StdoutSink::Log( const LogMessage &message )
    {
        std::lock_guard< std::mutex > lock{ m_Mutex };

        MemoryBuffer buffer;
        m_pFormatter->Format( message, buffer );

        platform::Print( buffer.data(), buffer.size() );
    }

    void StdoutSink::SetFormatter( Scope< Formatter > pFormatter )
    {
        std::lock_guard< std::mutex > lock{ m_Mutex };
        m_pFormatter = std::move( pFormatter );
    }
}