/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "script_class.h"

#include "script_engine.h"

#include <mono/jit/jit.h>

namespace smile::scripting
{
    ScriptClass::ScriptClass( const std::string &classNamespace, const std::string &className, bool isCore )
        : m_ClassNamespace{ classNamespace }, m_ClassName{ className }
    {
        m_pMonoClass =
            mono_class_from_name( isCore ? ScriptEngine::GetCoreAssemblyImage() : ScriptEngine::GetAppAssemblyImage(),
                classNamespace.c_str(),
                className.c_str() );
    }

    MonoObject *ScriptClass::Instantiate()
    {
        return ScriptEngine::InstantiateClass( m_pMonoClass );
    }

    MonoMethod *ScriptClass::GetMethod( const std::string &name, int parameterCount )
    {
        return mono_class_get_method_from_name( m_pMonoClass, name.c_str(), parameterCount );
    }

    MonoObject *ScriptClass::InvokeMethod( MonoObject *pInstance, MonoMethod *pMethod, void **ppParams )
    {
        return mono_runtime_invoke( pMethod, pInstance, ppParams, nullptr );
    }
}