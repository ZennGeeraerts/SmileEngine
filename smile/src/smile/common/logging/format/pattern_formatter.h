/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "formatter.h"

namespace smile::logging
{
    class PatternFormatter final : public Formatter
    {
      public:
        PatternFormatter();

        void Format( const LogMessage &message, MemoryBuffer &buffer ) override;

      private:
        std::vector< Scope< Formatter > > m_pFormatters;
    };
}