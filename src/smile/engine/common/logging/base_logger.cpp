/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "base_logger.h"

namespace smile::logging
{
    BaseLogger::BaseLogger( LogLevel level ) : m_Level{ level }
    {
    }

    void BaseLogger::SetLevel( LogLevel level )
    {
        m_Level.store( level );
    }

    bool BaseLogger::ShouldLog( LogLevel level ) const
    {
        return m_Level.load( std::memory_order_relaxed ) <= level;
    }
}