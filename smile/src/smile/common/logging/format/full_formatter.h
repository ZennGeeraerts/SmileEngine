/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "formatter.h"

namespace smile::logging
{
    class FullFormatter final : public Formatter
    {
      public:
        FullFormatter() = default;

        void Format( const LogMessage &message, MemoryBuffer &buffer ) override;
    };
}