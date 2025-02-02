/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "log_sink.h"

namespace smile::logging
{
    class StdoutColorSink final : public LogSink
    {
      public:
        StdoutColorSink();
        virtual ~StdoutColorSink() = default;

        void Log( const LogMessage &message ) override;

      private:
        std::unordered_map< LogLevel, Uint16 > m_ColorMap;
    };
}