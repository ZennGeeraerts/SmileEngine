/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "foundation/compiled.h"
#include "sink/log_sink.h"
#include "memory_buffer.h"

#define SM_NRM "\x1B[0m"
#define SM_RED "\x1B[31m"
#define SM_GRN "\x1B[32m"
#define SM_YEL "\x1B[33m"
#define SM_BLU "\x1B[34m"
#define SM_MAG "\x1B[35m"
#define SM_CYN "\x1B[36m"
#define SM_WHT "\x1B[37m"

namespace smile::logging
{
    class Logger final : public BaseLogger
    {
      public:
        Logger( const std::string &name ) : BaseLogger{ LogLevel::Info }, m_Name{ name }
        {
        }

        template < typename... Args >
        void Trace( fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Trace, message, std::forward< Args >( args )... );
        }

        template < typename... Args >
        void Debug( fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Debug, message, std::forward< Args >( args )... );
        }

        template < typename... Args >
        void Info( fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Info, message, std::forward< Args >( args )... );
        }

        template < typename... Args >
        void Warning( fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Warning, message, std::forward< Args >( args )... );
        }

        template < typename... Args >
        void Error( fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Error, message, std::forward< Args >( args )... );
        }

        template < typename... Args >
        void CriticalError( fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::CriticalError, message, std::forward< Args >( args )... );
        }

        void AddSink( Ref< LogSink > pSink );

        void SetFormatter( Scope< Formatter > pFormatter ) override;

      private:
        template < typename... Args >
        void Log( LogLevel level, fmt::format_string< Args... > message, Args &&...args )
        {
            Log( LogSource{}, level, message, std::forward< Args >( args )... );
        }

        template < typename... Args >
        void Log( LogSource source, LogLevel level, fmt::format_string< Args... > message, Args &&...args )
        {
            MemoryBuffer buffer;
            fmt::vformat_to( fmt::appender{ buffer }, message, fmt::make_format_args( args... ) );

            LogMessage logMessage{ m_Name, level, std::string_view{ buffer.data(), buffer.size() }, source };

            if ( ShouldLog( level ) )
                BroadcastToSinks( logMessage );
        }

        void BroadcastToSinks( const LogMessage &message );

      private:
        std::string m_Name;
        std::vector< Ref< LogSink > > m_pSinks;
    };
}