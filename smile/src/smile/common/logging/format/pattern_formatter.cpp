/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "pattern_formatter.h"

#include "full_formatter.h"
#include "platform/print.h"

namespace smile::logging
{
    PatternFormatter::PatternFormatter()
    {
        m_pFormatters.push_back( CreateScope< FullFormatter >() );
    }

    void PatternFormatter::Format( const LogMessage &message, MemoryBuffer &buffer )
    {
        for ( auto &pFormatter : m_pFormatters )
            pFormatter->Format( message, buffer );

        AppendStringView( buffer, platform::EOL() );
    }
}