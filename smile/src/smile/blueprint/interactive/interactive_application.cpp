/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "interactive_application.h"

namespace smile::interactive
{
    InteractiveApplication::InteractiveApplication( const application::ApplicationDescriptor &descriptor )
        : graphic::GraphicApplication{ descriptor }
    {
    }

    InteractiveApplication::~InteractiveApplication()
    {
    }
}