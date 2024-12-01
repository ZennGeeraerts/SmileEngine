/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "group_iterator.h"

namespace smile::ecs
{
    class ComponentInterface;

    class GroupBase
    {
        using OnEntityAddedListenerType = std::function< void( EntityHandleType ) >;
        using OnEntityRemovedListenerType = std::function< void( EntityHandleType ) >;

      public:
        GroupBase( ECSEngine &engine );
        virtual ~GroupBase() = default;

        virtual bool HasEntity( IndexType entityIndex ) const = 0;

        void AddEntity( EntityHandleType entityHandle );
        void RemoveEntity( EntityHandleType entityHandle );

        void AddOnEntityAddedListener( const OnEntityAddedListenerType *pOnEntityAddedListener )
        {
            m_pOnEntityAddedListeners.emplace_back( pOnEntityAddedListener );
        }
        void RemoveOnEntityAddedListener( const OnEntityAddedListenerType *pOnEntityAddedListener )
        {
            m_pOnEntityAddedListeners.erase( std::remove(
                m_pOnEntityAddedListeners.begin(), m_pOnEntityAddedListeners.end(), pOnEntityAddedListener ) );
        }

        void AddOnEntityRemovedListener( const OnEntityRemovedListenerType *pOnEntityRemovedListener )
        {
            m_pOnEntityRemovedListeners.emplace_back( pOnEntityRemovedListener );
        }
        void RemoveOnEntityRemovedListener( const OnEntityRemovedListenerType *pOnEntityRemovedListener )
        {
            m_pOnEntityRemovedListeners.erase( std::remove(
                m_pOnEntityRemovedListeners.begin(), m_pOnEntityRemovedListeners.end(), pOnEntityRemovedListener ) );
        }

        template < typename Component >
        bool HasComponent() const
        {
            auto pComponentInterface = m_Engine.GetComponentInterface< Component >();
            return HasComponent( pComponentInterface );
        }

        bool HasComponent( ComponentInterface *pComponent ) const;

        GroupIterator begin() const;
        GroupIterator end() const;

        const std::vector< ComponentInterface * > &GetOwnedComponents() const
        {
            return m_pOwnedPools;
        }
        const std::vector< ComponentInterface * > &GetGetComponents() const
        {
            return m_pGetPools;
        }

      protected:
        ECSEngine &m_Engine;

        std::vector< ComponentInterface * > m_pOwnedPools{};
        std::vector< ComponentInterface * > m_pGetPools{};

        IndexType m_EndIndex{ 0 };

      private:
        std::vector< const OnEntityAddedListenerType * > m_pOnEntityAddedListeners;
        std::vector< const OnEntityRemovedListenerType * > m_pOnEntityRemovedListeners;
    };
}