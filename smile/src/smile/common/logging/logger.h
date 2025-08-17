/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/common/foundation/compiled.h"
#include "smile/common/logging/sink/log_sink.h"
#include "smile/common/primitive/text/string.h"
#include "memory_buffer.h"

#include <format>

namespace smile::logging
{
    template < typename Type >
    concept StringLike =
        std::is_convertible_v< Type, std::string_view > || std::is_same_v< std::decay_t< Type >, const char * > ||
        std::is_same_v< std::decay_t< Type >, primitive::String > ||
        std::is_same_v< std::decay_t< Type >, primitive::StringView >;

    class Logger final : public BaseLogger
    {
      public:
        Logger( const std::string &name ) : BaseLogger{ LogLevel::Info }, m_Name{ name }
        {
        }

        template < typename... Args >
        void Trace( std::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Trace, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void Trace( const Type &message )
        {
            Log( LogLevel::Trace, message );
        }

        template < typename... Args >
        void Debug( std::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Debug, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void Debug( const Type &message )
        {
            Log( LogLevel::Debug, message );
        }

        template < typename... Args >
        void Info( std::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Info, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void Info( const Type &message )
        {
            Log( LogLevel::Info, message );
        }

        template < typename... Args >
        void Warning( std::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Warning, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void Warning( const Type &message )
        {
            Log( LogLevel::Warning, message );
        }

        template < typename... Args >
        void Error( std::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::Error, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void Error( const Type &message )
        {
            Log( LogLevel::Error, message );
        }

        template < typename... Args >
        void CriticalError( std::format_string< Args... > message, Args &&...args )
        {
            Log( LogLevel::CriticalError, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void CriticalError( const Type &message )
        {
            Log( LogLevel::CriticalError, message );
        }

        void AddSink( Ref< LogSink > pSink );

        void SetFormatter( Scope< Formatter > pFormatter ) override;

      private:
        template < typename... Args >
        void Log( LogSource source, LogLevel level, std::format_string< Args... > message, Args &&...args )
        {
            MemoryBuffer buffer;
            std::format_to( std::back_inserter( buffer ), message, std::forward< Args >( args )... );

            LogMessage logMessage{ m_Name, level, std::string_view{ buffer.data(), buffer.size() }, source };

            if ( ShouldLog( level ) )
                BroadcastToSinks( logMessage );
        }

        template < typename... Args >
        void Log( LogLevel level, std::format_string< Args... > message, Args &&...args )
        {
            Log( LogSource{}, level, message, std::forward< Args >( args )... );
        }

        template < StringLike Type >
        void Log( LogSource source, LogLevel level, const Type &message )
        {
            Log( source, level, "{}", message );
        }

        template < StringLike Type >
        void Log( LogLevel level, const Type &message )
        {
            Log( LogSource{}, level, message );
        }

        void BroadcastToSinks( const LogMessage &message );

      private:
        std::string m_Name;
        std::vector< Ref< LogSink > > m_pSinks;
    };
}