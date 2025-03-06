/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/application/graphic_application.h"

namespace smile::interactive
{
    class InteractiveApplication : public graphic::GraphicApplication
    {
      public:
        InteractiveApplication( const application::ApplicationDescriptor &descriptor );
        virtual ~InteractiveApplication();
    };
}