/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "interactive_application.h"

#include "smile/scripting/script_engine.h"

namespace smile::interactive
{
    InteractiveApplication::InteractiveApplication( const application::ApplicationDescriptor &descriptor )
        : graphic::GraphicApplication{ descriptor }
    {
        scripting::ScriptEngine::Initialize();
    }

    InteractiveApplication::~InteractiveApplication()
    {
        scripting::ScriptEngine::ShutDown();
    }
}