/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "component_interface.h"

namespace smile::ecs
{
    class ECSEngine;

    class GroupIterator final
    {
      public:
        GroupIterator( ECSEngine &engine, ComponentInterface::ConstIterator it );
        ~GroupIterator() = default;

        EntityHandleType operator*() const;

        bool operator==( const GroupIterator &other ) const;
        bool operator!=( const GroupIterator &other ) const;

        GroupIterator &operator++();

      private:
        ECSEngine &m_Engine;
        ComponentInterface::ConstIterator m_Iterator;
    };
}