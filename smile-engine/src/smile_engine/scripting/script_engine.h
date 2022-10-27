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
        static void initialize();
        static void shutDown();

        static void loadAssembly( const std::filesystem::path &file_path );

      private:
        static void initializeMono();
        static void shutDownMono();

        static MonoObject *instantiateClass( MonoClass *mono_class );

        friend class ScriptClass;
    };

    class ScriptClass final
    {
      public:
        ScriptClass() = default;
        ScriptClass( const std::string &class_namespace, const std::string &class_name );

        MonoObject *instantiate();
        MonoMethod *getMethod( const std::string &name, int parameter_count );
        MonoObject *invokeMethod( MonoObject *instance, MonoMethod *method, void **params = nullptr );

      private:
        std::string classNamespace;
        std::string className;
        MonoClass *monoClass = nullptr;
    };
}