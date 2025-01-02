/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/engine/core/scene/scene.h"
#include "smile/engine/core/scene/entity.h"

#include <filesystem>
#include <map>

extern "C"
{
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;
    typedef struct _MonoClassField MonoClassField;
}

namespace smile::scripting
{
    enum class ScriptFieldType
    {
        None = 0,
        Float,
        Double,
        Bool,
        Char,
        Byte,
        Short,
        Int,
        Long,
        UByte,
        UShort,
        UInt,
        ULong,
        Vector2,
        Vector3,
        Vector4,
        Entity
    };

    struct ScriptField final
    {
        ScriptFieldType Type;
        std::string Name;
        MonoClassField *pClassField;
    };

    class ScriptClass final
    {
      public:
        ScriptClass() = default;
        ScriptClass( const std::string &classNamespace, const std::string &className, bool isCore = false );

        MonoObject *Instantiate();
        MonoMethod *GetMethod( const std::string &name, int parameterCount );
        MonoObject *InvokeMethod( MonoObject *pInstance, MonoMethod *pMethod, void **ppParams = nullptr );

        const std::map< std::string, ScriptField > &GetFields() const
        {
            return m_Fields;
        }

      private:
        std::string m_ClassNamespace;
        std::string m_ClassName;

        std::map< std::string, ScriptField > m_Fields;

        MonoClass *m_pMonoClass = nullptr;

        friend class ScriptEngine;
    };

    class ScriptInstance final
    {
      public:
        ScriptInstance( Ref< ScriptClass > pScriptClass, scene::Entity entity );

        void InvokeOnCreate();
        void InvokeOnUpdate( float deltaTime );

        Ref< ScriptClass > GetScriptClass() const
        {
            return m_pScriptClass;
        }

        template < typename Type >
        Type GetFieldValue( const std::string &name )
        {
            bool hasSucceeded = GetFieldValueInternal( name, s_FieldValueBuffer );
            if ( !hasSucceeded )
                return Type{};

            return *reinterpret_cast< Type * >( s_FieldValueBuffer );
        }

        template < typename Type >
        void SetFieldValue( const std::string &name, const Type &value )
        {
            SetFieldValueInternal( name, &value );
        }

      private:
        bool GetFieldValueInternal( const std::string &name, void *pBuffer );
        bool SetFieldValueInternal( const std::string &name, const void *pValue );

      private:
        Ref< ScriptClass > m_pScriptClass;

        MonoObject *m_pInstance = nullptr;
        MonoMethod *m_pConstructor = nullptr;
        MonoMethod *m_pOnCreateMethod = nullptr;
        MonoMethod *m_pOnUpdateMethod = nullptr;

        inline static Byte s_FieldValueBuffer[8];
    };

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

        friend class ScriptClass;
        friend class ScriptGlue;
    };
}