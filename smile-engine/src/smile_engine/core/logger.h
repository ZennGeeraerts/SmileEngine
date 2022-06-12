#pragma once
#include "core.h"
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
        static void setPriority( LogPriority log_priority );

        template < typename... Args >
        static void logTrace( const char *message, Args... args )
        {
            log( LogPriority::Trace, "Trace", SM_NRM, message, args... );
        }

        template < typename... Args >
        static void logDebug( const char *message, Args... args )
        {
            log( LogPriority::Debug, "Debug", SM_CYN, message, args... );
        }

        template < typename... Args >
        static void logInfo( const char *message, Args... args )
        {
            log( LogPriority::Info, "Info", SM_GRN, message, args... );
        }

        template < typename... Args >
        static void logWarning( const char *message, Args... args )
        {
            log( LogPriority::Warning, "Warning", SM_YEL, message, args... );
        }

        template < typename... Args >
        static void logError( const char *message, Args... args )
        {
            log( LogPriority::Error, "Error", SM_MAG, message, args... );
        }

        template < typename... Args >
        static void logCriticalError( const char *message, Args... args )
        {
            log( LogPriority::CriticalError, "CriticalError", SM_RED, message, args... );
        }

      private:
        template < typename... Args >
        static void
        log( LogPriority log_priority, const char *log_name, const char *color, const char *message, Args... args )
        {
            if ( priority <= log_priority )
            {
                std::lock_guard< std::mutex > lock{ mutex };
                printf( "%s", color );
                printf( "[%s]\t", log_name );
                printf( message, args... );
                printf( "\n" );
                printf( "%s", SM_NRM );
            }
        }

        static LogPriority priority;
        static std::mutex mutex;
    };
}

// Macro's are used for the log functions, so the functionality can easily be removed if we want a distribution build
#define SM_LOG_TRACE( ... ) ::smile::Logger::logTrace( __VA_ARGS__ )
#define SM_LOG_DEBUG( ... ) ::smile::Logger::logDebug( __VA_ARGS__ )
#define SM_LOG_INFO( ... ) ::smile::Logger::logInfo( __VA_ARGS__ )
#define SM_LOG_WARNING( ... ) ::smile::Logger::logWarning( __VA_ARGS__ )
#define SM_LOG_ERROR( ... ) ::smile::Logger::logError( __VA_ARGS__ )
#define SM_LOG_CRITICALERROR( ... ) ::smile::Logger::logCriticalError( __VA_ARGS__ )