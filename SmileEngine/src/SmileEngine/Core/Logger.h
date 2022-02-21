#pragma once
#include "Core.h"
#include <mutex>

#define SM_NRM "\x1B[0m"
#define SM_RED "\x1B[31m"
#define SM_GRN "\x1B[32m"
#define SM_YEL "\x1B[33m"
#define SM_BLU "\x1B[34m"
#define SM_MAG "\x1B[35m"
#define SM_CYN "\x1B[36m"
#define SM_WHT "\x1B[37m"

namespace smile
{
    enum class LogPriority
    {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        STANDARD_ERROR,
        CRITICAL_ERROR
    };

    class Logger final
    {
      public:
        static void SetPriority( LogPriority logPriority );

        template < typename... Args >
        static void LogTrace( const char *message, Args... args )
        {
            Log( LogPriority::TRACE, "Trace", SM_NRM, message, args... );
        }

        template < typename... Args >
        static void LogDebug( const char *message, Args... args )
        {
            Log( LogPriority::DEBUG, "Debug", SM_CYN, message, args... );
        }

        template < typename... Args >
        static void LogInfo( const char *message, Args... args )
        {
            Log( LogPriority::INFO, "Info", SM_GRN, message, args... );
        }

        template < typename... Args >
        static void LogWarning( const char *message, Args... args )
        {
            Log( LogPriority::WARNING, "Warning", SM_YEL, message, args... );
        }

        template < typename... Args >
        static void LogError( const char *message, Args... args )
        {
            Log( LogPriority::STANDARD_ERROR, "Error", SM_MAG, message, args... );
        }

        template < typename... Args >
        static void LogCriticalError( const char *message, Args... args )
        {
            Log( LogPriority::CRITICAL_ERROR, "CriticalError", SM_RED, message, args... );
        }

      private:
        template < typename... Args >
        static void
        Log( LogPriority logPriority, const char *logName, const char *color, const char *message, Args... args )
        {
            if ( s_Priority <= logPriority )
            {
                std::lock_guard< std::mutex > lock{ s_Mutex };
                printf( "%s", color );
                printf( "[%s]\t", logName );
                printf( message, args... );
                printf( "\n" );
                printf( "%s", SM_NRM );
            }
        }

        static LogPriority s_Priority;
        static std::mutex s_Mutex;
    };
}

// Macro's are used for the log functions, so the functionality can easily be removed if we want a distribution build
#define SM_LOG_TRACE( ... ) ::smile::Logger::LogTrace( __VA_ARGS__ )
#define SM_LOG_DEBUG( ... ) ::smile::Logger::LogDebug( __VA_ARGS__ )
#define SM_LOG_INFO( ... ) ::smile::Logger::LogInfo( __VA_ARGS__ )
#define SM_LOG_WARNING( ... ) ::smile::Logger::LogWarning( __VA_ARGS__ )
#define SM_LOG_ERROR( ... ) ::smile::Logger::LogError( __VA_ARGS__ )
#define SM_LOG_CRITICALERROR( ... ) ::smile::Logger::LogCriticalError( __VA_ARGS__ )