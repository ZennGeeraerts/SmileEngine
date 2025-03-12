/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "logger.h"
#include "smile/common/foundation/singleton.h"

#include <unordered_map>

namespace smile::logging
{
    class LoggerRegistry final : public foundation::Singleton< LoggerRegistry >
    {
      public:
        LoggerRegistry();
        ~LoggerRegistry() = default;

        Ref< Logger > GetDefaultLogger() const
        {
            return m_pDefaultLogger;
        }

      private:
        std::unordered_map< std::string, Ref< Logger > > m_LoggerMap;
        Ref< Logger > m_pDefaultLogger;
    };
}