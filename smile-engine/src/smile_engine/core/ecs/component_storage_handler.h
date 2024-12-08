/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "component_storage.h"

namespace smile::ecs
{
    class ECSEngine;

    template < typename ComponentType >
    class ComponentStorageHandler final : public ComponentStorage
    {
      private:
        using ListenerType = std::function< void( ECSEngine &, EntityHandleType ) >;
        using ListenerContainer = std::vector< ListenerType >;

      public:
        ComponentStorageHandler( ECSEngine &engine ) : m_Engine{ engine }
        {
            Initialize< ComponentType >();
        }

        template < typename ComponentType, typename... Args >
        ComponentType &Append( IndexType index, Args &&...args )
        {
            ComponentType &component =
                ComponentStorage::Append< ComponentType, Args... >( index, std::forward< Args >( args )... );
            for ( const auto &listenerFunc : m_ContructionListeners )
            {
                auto entityHandle = m_Engine.GetEntityHandleManager().GetEntityHandle( index );
                listenerFunc( m_Engine, entityHandle );
            }
            return component;
        }

        IndexType RemoveSwap( IndexType deadEIndex ) override
        {
            for ( const auto &listenerFunc : m_DestructionListeners )
            {
                auto entityHandle = m_Engine.GetEntityHandleManager().GetEntityHandle( deadEIndex );
                listenerFunc( m_Engine, entityHandle );
            }
            return ComponentStorage::RemoveSwap( deadEIndex );
        }

        ComponentType *GetData()
        {
            return reinterpret_cast< ComponentType * >( m_pData );
        }
        const ComponentType *GetData() const
        {
            return reinterpret_cast< const ComponentType * >( m_pData );
        }

        ListenerContainer &OnConstruction()
        {
            return m_ContructionListeners;
        }

        ListenerContainer &OnDestruction()
        {
            return m_DestructionListeners;
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

      private:
        ECSEngine &m_Engine;

        ListenerContainer m_ContructionListeners;
        ListenerContainer m_DestructionListeners;
    };

    template < typename ComponentType >
    ComponentStorageHandler< ComponentType > *ComponentStorageCast( ComponentStorage *pComponentStorage )
    {
        // TODO: error checking
        return reinterpret_cast< ComponentStorageHandler< ComponentType > * >( pComponentStorage );
    }
}