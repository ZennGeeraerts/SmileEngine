/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "logger.h"

namespace smile::logging
{
    void Logger::AddSink( Ref< LogSink > pSink )
    {
        m_pSinks.emplace_back( pSink );
    }

    void Logger::SetFormatter( Scope< Formatter > pFormatter )
    {
        // TODO
    }

    void Logger::BroadcastToSinks( const LogMessage &message )
    {
        for ( auto pSink : m_pSinks )
        {
            if ( pSink->ShouldLog( message.Level ) )
                pSink->Log( message );
        }
    }
}