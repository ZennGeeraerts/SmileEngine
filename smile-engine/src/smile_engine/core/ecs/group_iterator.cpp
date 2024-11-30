/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "group_iterator.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    GroupIterator::GroupIterator( ECSEngine &engine, SparseSetType::ConstIterator it )
        : m_Engine{ engine }, m_Iterator{ it }
    {
    }

    EntityHandleType GroupIterator::operator*() const
    {
        return m_Engine.GetEntityHandleManager().GetEntityHandle( *m_Iterator );
    }

    bool GroupIterator::operator==( const GroupIterator &other ) const
    {
        return m_Iterator == other.m_Iterator || ( *m_Iterator ) == m_Engine.GetEntityHandleManager().GetEntityCount();
    }

    bool GroupIterator::operator!=( const GroupIterator &other ) const
    {
        return m_Iterator != other.m_Iterator && ( *m_Iterator ) != m_Engine.GetEntityHandleManager().GetEntityCount();
    }

    GroupIterator &GroupIterator::operator++()
    {
        ++m_Iterator;
        return *this;
    }
}