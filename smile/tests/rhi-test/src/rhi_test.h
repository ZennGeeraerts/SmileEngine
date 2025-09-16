/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/application/application.h"

namespace smile
{
    class RHITest final : public application::Application
    {
      public:
        RHITest( const application::ApplicationDescriptor &descriptor );
    };
}