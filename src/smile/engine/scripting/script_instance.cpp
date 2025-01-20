/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "script_instance.h"

#include "script_class.h"
#include "script_engine.h"

#include <mono/jit/jit.h>

namespace smile::scripting
{
    ScriptInstance::ScriptInstance( Ref< ScriptClass > pScriptClass, scene::Entity entity )
        : m_pScriptClass{ pScriptClass }
    {
        m_pInstance = pScriptClass->Instantiate();
        m_pConstructor = ScriptEngine::GetEntityClass().GetMethod( ".ctor", 1 );
        m_pOnCreateMethod = pScriptClass->GetMethod( "OnCreate", 0 );
        m_pOnUpdateMethod = pScriptClass->GetMethod( "OnUpdate", 1 );

        primitive::UUID entityID = entity.GetUUID();
        void *pParam = &entityID;
        pScriptClass->InvokeMethod( m_pInstance, m_pConstructor, &pParam );
    }

    void ScriptInstance::InvokeOnCreate()
    {
        if ( m_pOnCreateMethod )
            m_pScriptClass->InvokeMethod( m_pInstance, m_pOnCreateMethod );
    }

    void ScriptInstance::InvokeOnUpdate( float deltaTime )
    {
        if ( m_pOnUpdateMethod )
        {
            void *pParam = &deltaTime;
            m_pScriptClass->InvokeMethod( m_pInstance, m_pOnUpdateMethod, &pParam );
        }
    }

    bool ScriptInstance::GetFieldValueInternal( const std::string &name, void *pBuffer )
    {
        const auto &fields = m_pScriptClass->GetFields();
        auto it = fields.find( name );
        if ( it == fields.end() )
            return false;

        const ScriptField &field = it->second;
        mono_field_get_value( m_pInstance, field.pClassField, pBuffer );
        return true;
    }

    bool ScriptInstance::SetFieldValueInternal( const std::string &name, const void *pValue )
    {
        const auto &fields = m_pScriptClass->GetFields();
        auto it = fields.find( name );
        if ( it == fields.end() )
            return false;

        const ScriptField &field = it->second;
        mono_field_set_value( m_pInstance, field.pClassField, const_cast< void * >( pValue ) );
        return true;
    }
}