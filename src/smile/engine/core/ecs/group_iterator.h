/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "component_pool.h"

namespace smile::ecs
{
    class ECSEngine;

    class GroupIterator final
    {
      public:
        GroupIterator( ECSEngine &engine, ComponentPool::ConstIterator it );
        ~GroupIterator() = default;

        EntityHandleType operator*() const;

        bool operator==( const GroupIterator &other ) const;
        bool operator!=( const GroupIterator &other ) const;

        GroupIterator &operator++();

      private:
        ECSEngine &m_Engine;
        ComponentPool::ConstIterator m_Iterator;
    };
}