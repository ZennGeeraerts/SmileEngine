/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "stdout_color_sink.h"

#include "smile/common/platform/print.h"

#define SM_FOREGROUND_BLUE 0x0001      // Text color contains blue
#define SM_FOREGROUND_GREEN 0x0002     // Text color contains green
#define SM_FOREGROUND_RED 0x0004       // Text color contains red
#define SM_FOREGROUND_INTENSITY 0x0008 // Text color is intensified
#define SM_BACKGROUND_BLUE 0x0010      // Background color contains blue
#define SM_BACKGROUND_GREEN 0x0020     // Background color contains green
#define SM_BACKGROUND_RED 0x0040       // Background color contains red
#define SM_BACKGROUND_INTENSITY 0x0080 // Background color is intensified

namespace smile::logging
{
    StdoutColorSink::StdoutColorSink()
    {
        m_ColorMap[LogLevel::Trace] = SM_FOREGROUND_RED | SM_FOREGROUND_GREEN | SM_FOREGROUND_BLUE; // White
        m_ColorMap[LogLevel::Debug] = SM_FOREGROUND_GREEN | SM_FOREGROUND_BLUE;                     // Cyan
        m_ColorMap[LogLevel::Info] = SM_FOREGROUND_GREEN;                                           // Green
        m_ColorMap[LogLevel::Warning] =
            SM_FOREGROUND_RED | SM_FOREGROUND_GREEN | SM_FOREGROUND_INTENSITY;     // Intense yellow
        m_ColorMap[LogLevel::Error] = SM_FOREGROUND_RED | SM_FOREGROUND_INTENSITY; // Intense red
        m_ColorMap[LogLevel::CriticalError] = SM_BACKGROUND_RED | SM_FOREGROUND_RED | SM_FOREGROUND_GREEN |
                                              SM_FOREGROUND_BLUE |
                                              SM_FOREGROUND_INTENSITY; // Intense white on red background
    }

    void StdoutColorSink::Log( const LogMessage &message )
    {
        std::lock_guard< std::mutex > lock{ m_Mutex };

        MemoryBuffer buffer;
        m_pFormatter->Format( message, buffer );

        auto originalColor = platform::SetConsoleColor( m_ColorMap[message.Level] );
        platform::Print( buffer.data(), buffer.size() );
        platform::SetConsoleColor( originalColor );
    }
}