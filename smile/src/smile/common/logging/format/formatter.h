/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/logging/memory_buffer.h"
#include "smile/common/logging/log_message.h"

namespace smile::logging
{
    class Formatter
    {
      public:
        Formatter() = default;
        virtual ~Formatter() = default;

        virtual void Format( const LogMessage &message, MemoryBuffer &buffer ) = 0;
    };
}