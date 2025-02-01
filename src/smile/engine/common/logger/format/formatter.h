/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "logger/memory_buffer.h"
#include "logger/log_message.h"

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