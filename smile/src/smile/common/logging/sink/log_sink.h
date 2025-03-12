/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/logging/base_logger.h"
#include "smile/common/logging/log_message.h"

#include <mutex>

namespace smile::logging
{
    class LogSink : public BaseLogger
    {
      public:
        LogSink();
        virtual ~LogSink() = default;

        virtual void Log( const LogMessage &message ) = 0;
        void SetFormatter( Scope< Formatter > pFormatter ) override;

      protected:
        std::mutex &m_Mutex;
        Scope< Formatter > m_pFormatter;
    };
}