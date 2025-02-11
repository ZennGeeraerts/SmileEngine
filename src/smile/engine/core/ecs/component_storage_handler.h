/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "component_storage.h"

namespace smile::ecs
{
    template < typename ComponentType >
    class ComponentStorageHandler final : public ComponentStorage
    {
      public:
        ComponentStorageHandler()
        {
            Initialize< ComponentType >();
        }

        ComponentType *GetData()
        {
            return reinterpret_cast< ComponentType * >( m_pData );
        }
        const ComponentType *GetData() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData );
        }

        inline ComponentType &operator[]( int index )
        {
            return reinterpret_cast< ComponentType * >( m_pData )[index];
        }
        inline const ComponentType &operator[]( int index ) const
        {
            return reinterpret_cast< const ComponentType * >( m_pData )[index];
        }

        inline ComponentType *begin()
        {
            return reinterpret_cast< ComponentType * >( m_pData );
        }
        inline const ComponentType *begin() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData );
        }
        inline ComponentType *end()
        {
            return reinterpret_cast< ComponentType * >( m_pData + m_Size );
        }
        inline const ComponentType *end() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData + m_Size );
        }
    };

    template < typename ComponentType >
    ComponentStorageHandler< ComponentType > *ComponentStorageCast( ComponentStorage *pComponentStorage )
    {
        // TODO: error checking
        return reinterpret_cast< ComponentStorageHandler< ComponentType > * >( pComponentStorage );
    }
}