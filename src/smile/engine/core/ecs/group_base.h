/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "group_iterator.h"

namespace smile::ecs
{
    class ComponentPool;

    class GroupBase
    {
      public:
        GroupBase( ECSEngine &engine,
            const std::vector< ComponentPool * > &pOwned,
            const std::vector< ComponentPool * > &pGet );

        virtual ~GroupBase() = default;

        virtual bool ContainsEntity( EntityHandle entityHandle ) const = 0;

        void AddEntity( EntityHandle entityHandle );
        void RemoveEntity( EntityHandle entityHandle );

        template < typename Component >
        bool ContainsComponentPool() const
        {
            auto pPool = m_Engine.GetComponentPool< Component >();
            return ContainsComponentPool( pPool );
        }

        bool ContainsComponentPool( ComponentPool *pPool ) const;

        GroupIterator begin() const;
        GroupIterator end() const;

        const std::vector< ComponentPool * > &GetOwnedPools() const
        {
            return m_pOwnedPools;
        }
        const std::vector< ComponentPool * > &GetGetPools() const
        {
            return m_pGetPools;
        }

      protected:
        ECSEngine &m_Engine;

        std::vector< ComponentPool * > m_pOwnedPools{};
        std::vector< ComponentPool * > m_pGetPools{};

        IndexType m_EndIndex{ 0 };
    };
}