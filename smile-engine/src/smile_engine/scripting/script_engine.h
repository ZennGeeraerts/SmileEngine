#pragma once

#include <filesystem>

extern "C"
{
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}

namespace smile::scripting
{
    class ScriptEngine final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void LoadAssembly( const std::filesystem::path &filePath );

      private:
        static void InitializeMono();
        static void ShutDownMono();

        static MonoObject *InstantiateClass( MonoClass *pMonoClass );

        friend class ScriptClass;
    };

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
}