/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"
#include "group_iterator.h"

namespace smile::ecs
{
    class ComponentPool;

    class GroupBase
    {
      public:
        GroupBase( ECSEngine &engine,
            const primitive::Vector< ComponentPool * > &pOwned,
            const primitive::Vector< ComponentPool * > &pGet );

        virtual ~GroupBase() = default;

        virtual bool ContainsEntity( EntityHandle entityHandle ) const = 0;

        void AddEntity( EntityHandle entityHandle );
        void RemoveEntity( EntityHandle entityHandle );

        bool ContainsComponentPool( ComponentPool *pPool ) const;

        GroupIterator begin() const;
        GroupIterator end() const;

        const primitive::Vector< ComponentPool * > &GetOwnedPools() const
        {
            return m_pOwnedPools;
        }
        const primitive::Vector< ComponentPool * > &GetGetPools() const
        {
            return m_pGetPools;
        }

      protected:
        ECSEngine &m_Engine;

        primitive::Vector< ComponentPool * > m_pOwnedPools{};
        primitive::Vector< ComponentPool * > m_pGetPools{};

        IndexType m_EndIndex{ 0 };
    };
}