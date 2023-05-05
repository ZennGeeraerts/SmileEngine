/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/scene/scene.h"
#include "smile_engine/scene/entity.h"

#include <filesystem>

extern "C"
{
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;
}

namespace smile::scripting
{
    class ScriptClass final
    {
      public:
        ScriptClass() = default;
        ScriptClass( const std::string &classNamespace, const std::string &className );

        MonoObject *Instantiate();
        MonoMethod *GetMethod( const std::string &name, int parameterCount );
        MonoObject *InvokeMethod( MonoObject *pInstance, MonoMethod *pMethod, void **ppParams = nullptr );

      private:
        std::string m_ClassNamespace;
        std::string m_ClassName;
        MonoClass *m_pMonoClass = nullptr;
    };

    class ScriptInstance final
    {
      public:
        ScriptInstance( Ref< ScriptClass > pScriptClass, scene::Entity entity );

        void InvokeOnCreate();
        void InvokeOnUpdate( float deltaTime );

      private:
        Ref< ScriptClass > m_pScriptClass;

        MonoObject *m_pInstance = nullptr;
        MonoMethod *m_pConstructor = nullptr;
        MonoMethod *m_pOnCreateMethod = nullptr;
        MonoMethod *m_pOnUpdateMethod = nullptr;
    };

    class ScriptEngine final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void LoadAssembly( const std::filesystem::path &filePath );

        static void OnRuntimeStart( scene::Scene *pScene );
        static void OnRuntimeStop();

        static bool EntityClassExists( const std::string &fullClassName );
        static void OnCreateEntity( scene::Entity entity );
        static void OnUpdateEntity( scene::Entity entity, Timestep deltaTime );

        static scene::Scene *GetSceneContext();
        static std::unordered_map< std::string, Ref< ScriptClass > > GetEntityClasses();

        static MonoImage *GetCoreAssemblyImage();

      private:
        static void InitializeMono();
        static void ShutDownMono();

        static MonoObject *InstantiateClass( MonoClass *pMonoClass );
        static void LoadAssemblyClasses( MonoAssembly *pAssembly );

        friend class ScriptClass;
        friend class ScriptGlue;
    };
}