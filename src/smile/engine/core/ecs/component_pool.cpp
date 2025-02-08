/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "component_pool.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    ComponentPool::ComponentPool( const ECSEngine &ecsEngine )
        : m_ECSEngine{ ecsEngine }
    {
    }

    ComponentPool::~ComponentPool()
    {
        SAFE_DELETE( m_pComponentStorage );
    }

    EntityHandleType ComponentPool::GetEntityHandle( IndexType index ) const
    {
        auto entityIndex = m_SparseSet.GetElement( index );
        return m_ECSEngine.GetEntityHandleManager().GetEntityHandle( entityIndex );
    }
}