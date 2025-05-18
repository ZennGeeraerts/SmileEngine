/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "assert.h"

namespace smile::foundation
{
    template < typename ObjectType >
    class Singleton
    {
      public:
        static bool HasInstance()
        {
            return m_pObject;
        }

        static ObjectType &GetInstance()
        {
            SM_ASSERT_MSG( m_pObject, "Object is nullptr" );
            return *m_pObject;
        }

        static void CreateInstance()
        {
            SM_ASSERT_MSG( !m_pObject, "Object already exists" );

            m_pObject = new ObjectType{};
            m_pObject->IncreaseRefCount();
        }

        static void SetInstance( ObjectType &instance )
        {
            SM_ASSERT_MSG( !m_pObject, "Object already exists" );

            m_pObject = &instance;
            m_pObject->IncreaseRefCount();
        }

        static void RemoveInstance()
        {
            SM_ASSERT_MSG( m_pObject->GetRefCount() == 1, "Object has multiple references" );

            m_pObject->DecreaseRefCount();
            m_pObject = nullptr;
        }

      protected:
        Singleton() = default;
        Singleton( const Singleton & ) = delete;
        Singleton( Singleton && ) = delete;
        ~Singleton() = default;
        Singleton &operator=( const Singleton & ) = delete;
        Singleton &operator=( Singleton && ) = delete;

      protected:
        static ObjectType *m_pObject;
    };

    template < typename ObjectType >
    ObjectType *Singleton< ObjectType >::m_pObject{ nullptr };
}