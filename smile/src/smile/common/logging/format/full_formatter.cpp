/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "full_formatter.h"

#include <string_view>

namespace smile::logging
{
    void FullFormatter::Format( const LogMessage &message, MemoryBuffer &buffer )
    {
        buffer.push_back( '[' );

        primitive::StringView level;
        switch ( message.Level )
        {
            case LogLevel::Trace:
                level = "Trace";
                break;

            case LogLevel::Debug:
                level = "Debug";
                break;

            case LogLevel::Info:
                level = "Info";
                break;

            case LogLevel::Warning:
                level = "Warning";
                break;

            case LogLevel::Error:
                level = "Error";
                break;

            case LogLevel::CriticalError:
                level = "CriticalError";
                break;

            default:
                break;
        }

        AppendStringView( buffer, level );
        buffer.push_back( ']' );
        buffer.push_back( ' ' );

        AppendStringView( buffer, message.Payload );
    }
}