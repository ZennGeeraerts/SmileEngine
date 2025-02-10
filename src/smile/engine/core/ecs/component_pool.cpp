/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "component_pool.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    ComponentPool::ComponentPool( ECSEngine &ecsEngine ) : m_ECSEngine{ ecsEngine }
    {
    }

    ComponentPool::~ComponentPool()
    {
        SAFE_DELETE( m_pComponentStorage );
    }

    void ComponentPool::Remove(EntityHandleType entityHandle)
    {
        void *pComponentData = GetRaw( entityHandle );

        if ( !pComponentData )
            return;

        const IndexType deadEntityIndex = m_SparseSet.GetIndex( entityHandle.GetIndex() );
        m_pComponentStorage->RemoveSwap( deadEntityIndex );
        m_SparseSet.Erase( entityHandle.GetIndex() );
    }

    EntityHandleType ComponentPool::GetEntityHandle( IndexType index ) const
    {
        auto entityIndex = m_SparseSet.GetElement( index );
        return m_ECSEngine.GetEntityHandleManager().GetEntityHandle( entityIndex );
    }

    void ComponentPool::Sort( std::function< bool( const IndexType, const IndexType ) > compare )
    {
        SparseSetType sparseSetCopy = m_SparseSet;

        std::sort( sparseSetCopy.m_Dense.begin(), sparseSetCopy.m_Dense.end(), std::move( compare ) );

        for ( std::size_t pos{}; pos < sparseSetCopy.GetItemCount(); ++pos )
        {
            auto curr = pos;
            auto next = m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]];

            while ( curr != next )
            {
                std::swap( m_SparseSet.m_Dense[m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]]],
                    m_SparseSet.m_Dense[m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[next]]] );

                m_pComponentStorage->Swap( m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]],
                    m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[next]] );

                m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]] = curr;
                curr = next;
                next = m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]];
            }
        }
    }
}