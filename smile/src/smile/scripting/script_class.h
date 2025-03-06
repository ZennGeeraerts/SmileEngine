/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "script_field.h"

#include <map>

extern "C"
{
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}

namespace smile::scripting
{
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
}