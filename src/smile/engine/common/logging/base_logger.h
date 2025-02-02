/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "log_level.h"
#include "format/formatter.h"

namespace smile::logging
{
    class BaseLogger
    {
      public:
        BaseLogger( LogLevel level );
        virtual ~BaseLogger() = default;

        void SetLevel( LogLevel level );
        bool ShouldLog( LogLevel level ) const;

        virtual void SetFormatter( Scope< Formatter > pFormatter ) = 0;

      protected:
        std::atomic< LogLevel > m_Level;
    };
}