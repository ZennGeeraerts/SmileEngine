/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <mutex>

namespace smile::logging
{
    struct ConsoleMutex final
    {
        static std::mutex &GetMutex()
        {
            static std::mutex mutex;
            return mutex;
        }
    };
}