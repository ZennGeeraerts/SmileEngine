/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <stdexcept>

namespace smile::foundation
{
    class AssertFailedException final : public std::runtime_error
    {
      public:
        explicit AssertFailedException( const char *condition ) : std::runtime_error{ condition }
        {
        }
    };
}