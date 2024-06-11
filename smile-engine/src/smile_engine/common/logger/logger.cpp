/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "logger.h"

namespace smile::logger
{
    LogPriority Logger::m_Priority = LogPriority::Trace;
    std::mutex Logger::m_Mutex{};

    void Logger::SetPriority( LogPriority logPriority )
    {
        m_Priority = logPriority;
    }
}