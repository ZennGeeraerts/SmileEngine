/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "log_sink.h"
#include "console_mutex.h"

namespace smile::logging
{
    class StdoutSink final : public LogSink
    {
      public:
        StdoutSink();
        virtual ~StdoutSink() = default;

        void Log( const LogMessage &message ) override;

      private:
        std::mutex &m_Mutex;
    };
}