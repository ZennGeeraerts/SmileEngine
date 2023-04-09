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