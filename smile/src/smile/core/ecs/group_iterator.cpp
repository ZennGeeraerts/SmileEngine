/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "group_iterator.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    GroupIterator::GroupIterator( ECSEngine &engine, ComponentPool::ConstIterator it )
        : m_Engine{ engine }, m_Iterator{ it }
    {
    }

    EntityHandle GroupIterator::operator*() const
    {
        return m_Engine.GetEntityHandleManager().GetHandle( *m_Iterator );
    }

    bool GroupIterator::operator==( const GroupIterator &other ) const
    {
        return m_Iterator == other.m_Iterator || ( *m_Iterator ) == m_Engine.GetEntityHandleManager().GetHandleCount();
    }

    bool GroupIterator::operator!=( const GroupIterator &other ) const
    {
        return m_Iterator != other.m_Iterator && ( *m_Iterator ) != m_Engine.GetEntityHandleManager().GetHandleCount();
    }

    GroupIterator &GroupIterator::operator++()
    {
        ++m_Iterator;
        return *this;
    }
}