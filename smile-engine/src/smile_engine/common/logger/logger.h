/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/common/compiled/compiled.h"
#include <mutex>

#define SM_NRM "\x1B[0m"
#define SM_RED "\x1B[31m"
#define SM_GRN "\x1B[32m"
#define SM_YEL "\x1B[33m"
#define SM_BLU "\x1B[34m"
#define SM_MAG "\x1B[35m"
#define SM_CYN "\x1B[36m"
#define SM_WHT "\x1B[37m"

namespace smile::logger
{
    enum class LogPriority
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        CriticalError
    };

    class Logger final
    {
      public:
        static void SetPriority( LogPriority logPriority );

        template < typename... Args >
        static void LogTrace( const char *message, Args... args )
        {
            Log( LogPriority::Trace, "Trace", SM_NRM, message, args... );
        }

        template < typename... Args >
        static void LogDebug( const char *message, Args... args )
        {
            Log( LogPriority::Debug, "Debug", SM_CYN, message, args... );
        }

        template < typename... Args >
        static void LogInfo( const char *message, Args... args )
        {
            Log( LogPriority::Info, "Info", SM_GRN, message, args... );
        }

        template < typename... Args >
        static void LogWarning( const char *message, Args... args )
        {
            Log( LogPriority::Warning, "Warning", SM_YEL, message, args... );
        }

        template < typename... Args >
        static void LogError( const char *message, Args... args )
        {
            Log( LogPriority::Error, "Error", SM_MAG, message, args... );
        }

        template < typename... Args >
        static void LogCriticalError( const char *message, Args... args )
        {
            Log( LogPriority::CriticalError, "CriticalError", SM_RED, message, args... );
        }

      private:
        template < typename... Args >
        static void
        Log( LogPriority logPriority, const char *logName, const char *color, const char *message, Args... args )
        {
            if ( m_Priority <= logPriority )
            {
                std::lock_guard< std::mutex > lock{ m_Mutex };
                printf( "%s", color );
                printf( "[%s]\t", logName );
                printf( message, args... );
                printf( "\n" );
                printf( "%s", SM_NRM );
            }
        }

      private:
        static LogPriority m_Priority;
        static std::mutex m_Mutex;
    };
}

// Macro's are used for the log functions, so the functionality can easily be removed if we want a distribution build
#define SM_LOG_TRACE( ... ) ::smile::logger::Logger::LogTrace( __VA_ARGS__ )
#define SM_LOG_DEBUG( ... ) ::smile::logger::Logger::LogDebug( __VA_ARGS__ )
#define SM_LOG_INFO( ... ) ::smile::logger::Logger::LogInfo( __VA_ARGS__ )
#define SM_LOG_WARNING( ... ) ::smile::logger::Logger::LogWarning( __VA_ARGS__ )
#define SM_LOG_ERROR( ... ) ::smile::logger::Logger::LogError( __VA_ARGS__ )
#define SM_LOG_CRITICALERROR( ... ) ::smile::logger::Logger::LogCriticalError( __VA_ARGS__ )