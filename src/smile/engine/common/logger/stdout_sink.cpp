/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "stdout_sink.h"

#include "platform/print.h"

namespace smile::logging
{
    StdoutSink::StdoutSink() : m_Mutex{ ConsoleMutex::GetMutex() }
    {
    }

    void StdoutSink::Log( const LogMessage &message )
    {
        std::lock_guard< std::mutex > lock{ m_Mutex };

        MemoryBuffer buffer;
        AppendStringView( buffer, message.Payload );
        AppendStringView( buffer, platform::EOL() );

        platform::Print( buffer.data(), buffer.size() );
    }
}