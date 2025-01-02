/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "compiled.h"

namespace smile::compiled
{
    template < typename ObjectType >
    class PImpl
    {
      public:
        PImpl() : m_pObject{ new ObjectType{} }
        {
        }

        ~PImpl() = default;

        ObjectType &operator*()
        {
            return *m_pObject;
        }

        const ObjectType &operator*() const
        {
            return *m_pObject;
        }

        ObjectType *operator->()
        {
            return &*m_pObject;
        }

        const ObjectType *operator->() const
        {
            return &*m_pObject;
        }

        ObjectType *Get()
        {
            return m_pObject.get();
        }

      private:
        Scope< ObjectType > m_pObject;
    };
}