/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/core/scene/scene.h"
#include "engine/core/scene/entity.h"

#include "script_class.h"
#include "script_instance.h"

#include <filesystem>
#include <unordered_map>

extern "C"
{
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoImage MonoImage;
}

namespace smile::scripting
{
    class ScriptEngine final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void LoadAssembly( const std::filesystem::path &filePath );
        static void LoadAppAssembly( const std::filesystem::path &filePath );

        static void OnRuntimeStart( scene::Scene *pScene );
        static void OnRuntimeStop();

        static bool EntityClassExists( const std::string &fullClassName );
        static void OnCreateEntity( scene::Entity entity );
        static void OnUpdateEntity( scene::Entity entity, primitive::Timestep deltaTime );

        static scene::Scene *GetSceneContext();
        static Ref< ScriptInstance > GetEntityScriptInstance( primitive::UUID entityID );

        static std::unordered_map< std::string, Ref< ScriptClass > > GetEntityClasses();

        static MonoImage *GetCoreAssemblyImage();

      private:
        static void InitializeMono();
        static void ShutDownMono();

        static MonoObject *InstantiateClass( MonoClass *pMonoClass );
        static void LoadAssemblyClasses();

        static MonoImage *GetAppAssemblyImage();
        static ScriptClass &GetEntityClass();

        friend class ScriptClass;
        friend class ScriptInstance;
    };
}