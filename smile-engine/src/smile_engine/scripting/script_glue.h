/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::scripting
{
	class ScriptGlue final
    {
      public:
        static void RegisterComponentTypes();
        static void RegisterFunctions();
    };
}