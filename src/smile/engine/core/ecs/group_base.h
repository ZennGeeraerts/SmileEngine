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
      public:
        GroupBase( ECSEngine &engine,
            const std::vector< ComponentInterface * > &pOwned,
            const std::vector< ComponentInterface * > &pGet );

        virtual ~GroupBase() = default;

        virtual bool HasEntity( EntityHandleType entityHandle ) const = 0;

        void AddEntity( EntityHandleType entityHandle );
        void RemoveEntity( EntityHandleType entityHandle );

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
    };
}