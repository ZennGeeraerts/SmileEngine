/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "logger.h"
#include "smile/common/foundation/meyers_singleton.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::logging
{
    class LoggerRegistry final : public foundation::MeyersSingleton< LoggerRegistry >
    {
      public:
        LoggerRegistry();
        ~LoggerRegistry() = default;

        Ref< Logger > GetDefaultLogger() const
        {
            return m_pDefaultLogger;
        }

      private:
        primitive::HashMap< primitive::String, Ref< Logger > > m_LoggerMap;
        Ref< Logger > m_pDefaultLogger;
    };
}