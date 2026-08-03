/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "component_pool.h"

#include "ecs_engine.h"

namespace smile::ecs
{
    ComponentPool::ComponentPool( ECSEngine &ecsEngine ) noexcept : m_ECSEngine{ ecsEngine }
    {
    }

    void ComponentPool::Remove( EntityHandle entityHandle )
    {
        void *pComponentData = GetRaw( entityHandle );

        if ( !pComponentData )
            return;

        PublishOnDestruction( entityHandle );

        const IndexType deadEntityIndex = m_SparseSet.GetIndex( entityHandle.GetIndex() );

        m_ComponentStorage->RemoveSwap( deadEntityIndex );
        m_SparseSet.Erase( entityHandle.GetIndex() );
        m_ComponentMetadata.EraseAtIndex( deadEntityIndex );
    }

    EntityHandle ComponentPool::GetEntityHandle( IndexType index ) const
    {
        auto entityIndex = m_SparseSet.GetElement( index );
        return m_ECSEngine.GetEntityHandleManager().GetHandle( entityIndex );
    }

    void ComponentPool::Sort( std::function< bool( const IndexType, const IndexType ) > compare )
    {
        SparseSetType sparseSetCopy = m_SparseSet;

        std::sort( sparseSetCopy.m_Dense.begin(), sparseSetCopy.m_Dense.end(), std::move( compare ) );

        for ( Index pos{}; pos < sparseSetCopy.GetItemCount(); ++pos )
        {
            auto curr = pos;
            auto next = m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]];

            while ( curr != next )
            {
                const auto currIndex = m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]];
                const auto nextIndex = m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[next]];

                std::swap( m_SparseSet.m_Dense[currIndex], m_SparseSet.m_Dense[nextIndex] );

                m_ComponentStorage->Swap( currIndex, nextIndex );
                primitive::array::Swap( m_ComponentMetadata, currIndex, nextIndex );

                m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]] = curr;
                curr = next;
                next = m_SparseSet.m_Sparse[sparseSetCopy.m_Dense[curr]];
            }
        }
    }

    void ComponentPool::PublishOnConstruction( const EntityHandle entityHandle )
    {
        for ( const auto &listenerFunc : m_ConstructionListeners )
        {
            listenerFunc( m_ECSEngine, entityHandle );
        }
    }

    void ComponentPool::PublishOnDestruction( const EntityHandle entityHandle )
    {
        for ( const auto &listenerFunc : m_DestructionListeners )
        {
            listenerFunc( m_ECSEngine, entityHandle );
        }
    }

    void ComponentPool::PublishOnPatch( const EntityHandle entityHandle )
    {
        for ( const auto &listenerFunc : m_PatchListeners )
        {
            listenerFunc( m_ECSEngine, entityHandle );
        }
    }

    bool ComponentPool::HasChanged( EntityHandle entityHandle ) const
    {
        const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

        if ( index == EntityHandle::NullHandle().GetIndex() || !m_ComponentMetadata.IsValidIndex( index ) )
            return false;

        return m_ComponentMetadata[index].LastModified > m_ECSEngine.GetLastTick();
    }
}