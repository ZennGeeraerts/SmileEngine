/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "primitive/collection/sparse_set.h"
#include "component_storage_handler.h"

#include <functional>

namespace smile::ecs
{
    class ECSEngine;

    class ComponentPool final
    {
      public:
        using Iterator = SparseSetType::Iterator;
        using ConstIterator = SparseSetType::ConstIterator;

      private:
        using ListenerType = std::function< void( ECSEngine &, EntityHandleType ) >;
        using ListenerContainer = std::vector< ListenerType >;

      public:
        ComponentPool( ECSEngine &ecsEngine );
        ~ComponentPool();

        template < typename ComponentType >
        void Initialize()
        {
            SM_ASSERT( !m_pComponentStorage, "ComponentPool::Initialize > Storage already created" );

            m_pComponentStorage = new ComponentStorageHandler< ComponentType >{};
        }

        template < typename ComponentType, typename... ConstructorArgs >
        ComponentType &Add( EntityHandleType entityHandle, ConstructorArgs &&...constructorArgs )
        {
            const IndexType index = m_SparseSet.Insert( entityHandle.GetIndex() );

            SM_ASSERT( index == m_pComponentStorage->GetSize(), "ComponentPool::Add > Failed to add component" );

            auto &component = m_pComponentStorage->Append< ComponentType >(
                entityHandle.GetIndex(), std::forward< ConstructorArgs >( constructorArgs )... );

            for ( const auto &listenerFunc : m_ContructionListeners )
            {
                auto entityHandle = m_ECSEngine.GetEntityHandleManager().GetEntityHandle( index );
                listenerFunc( m_ECSEngine, entityHandle );
            }

            return component;
        }

        void Remove( EntityHandleType entityHandle );

        template < typename ComponentType >
        ComponentType &Get( EntityHandleType entityHandle )
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            SM_ASSERT( index != EntityHandleType::NullHandle().GetIndex(), "ComponentPool::Get > Invalid index" );

            return m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType &Get( EntityHandleType entityHandle ) const
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            return m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        ComponentType *TryGet( EntityHandleType entityHandle )
        {
            if ( !m_SparseSet.Contains( entityHandle.GetIndex() ) )
                return nullptr;

            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            if ( index == EntityHandleType::NullHandle().GetIndex() )
                return nullptr;

            return &m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType *TryGet( EntityHandleType entityHandle ) const
        {
            if ( !m_SparseSet.Contains( entityHandle.GetIndex() ) )
                return nullptr;

            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );

            if ( index == EntityHandleType::NullHandle().GetIndex() )
                return nullptr;

            return &m_pComponentStorage->Get< ComponentType >( index );
        }

        void *GetRaw( EntityHandleType entityHandle )
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            return m_pComponentStorage->GetRaw( index );
        }

        const void *GetRaw( EntityHandleType entityHandle ) const
        {
            const IndexType index = m_SparseSet.GetIndex( entityHandle.GetIndex() );
            return m_pComponentStorage->GetRaw( index );
        }

        void Clear()
        {
            m_pComponentStorage->Clear();
            m_SparseSet.Clear();
        }

        bool Contains( EntityHandleType entityHandle ) const
        {
            return m_SparseSet.Contains( entityHandle.GetIndex() );
        }

        void Swap( IndexType lhs, IndexType rhs )
        {
            m_SparseSet.Swap( m_SparseSet.GetElement( lhs ), m_SparseSet.GetElement( rhs ) );
            m_pComponentStorage->Swap( lhs, rhs );
        }

        Uint32 GetItemCount() const
        {
            return m_SparseSet.GetItemCount();
        }

        IndexType GetIndex( EntityHandleType entityHandle ) const
        {
            return m_SparseSet.GetIndex( entityHandle.GetIndex() );
        }

        EntityHandleType GetEntityHandle( IndexType index ) const;
        void Sort( std::function< bool( const IndexType, const IndexType ) > compare );

        ListenerContainer &OnConstruction()
        {
            return m_ContructionListeners;
        }

        ListenerContainer &OnDestruction()
        {
            return m_DestructionListeners;
        }

        ConstIterator begin() const
        {
            return m_SparseSet.begin();
        }

        ConstIterator end() const
        {
            return m_SparseSet.end();
        }

      public:
        ECSEngine &m_ECSEngine;
        SparseSetType m_SparseSet{};
        ComponentStorage *m_pComponentStorage = nullptr;

        ListenerContainer m_ContructionListeners;
        ListenerContainer m_DestructionListeners;
    };
}