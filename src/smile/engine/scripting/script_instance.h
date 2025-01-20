/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "scene/entity.h"

extern "C"
{
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}

namespace smile::scripting
{
    class ScriptClass;

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
}