/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/primitive/collection/sparse_set.h"
#include "component_storage.h"

#include <functional>

namespace smile::ecs
{
    class ComponentInterface final
    {
      public:
        using Iterator = SparseSetType::Iterator;
        using ConstIterator = SparseSetType::ConstIterator;

      private:
        using CreateHandler = std::function< void( EntityHandleType, void * ) >;
        using DestroyHandler = std::function< void( void * ) >;

      public:
        ~ComponentInterface()
        {
            SAFE_DELETE( m_pComponentStorage );
        }

        template < typename ComponentType >
        ComponentType &Get( EntityHandleType entityHandle )
        {
            const IndexType index = m_Pool.GetIndex( entityHandle.GetIndex() );

            SM_ASSERT( index != EntityHandleType::NullHandle().GetIndex(), "ComponentInterface::Get > Invalid index" );

            return m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType &Get( EntityHandleType entityHandle ) const
        {
            const IndexType index = m_Pool.GetIndex( entityHandle.GetIndex() );
            return m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        ComponentType *TryGet( EntityHandleType entityHandle )
        {
            if ( !m_Pool.Contains( entityHandle.GetIndex() ) )
                return nullptr;

            const IndexType index = m_Pool.GetIndex( entityHandle.GetIndex() );

            if ( index == EntityHandleType::NullHandle().GetIndex() )
                return nullptr;

            return &m_pComponentStorage->Get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType *TryGet( EntityHandleType entityHandle ) const
        {
            if ( !m_Pool.Contains( entityHandle.GetIndex() ) )
                return nullptr;

            const IndexType index = m_Pool.GetIndex( entityHandle.GetIndex() );

            if ( index == EntityHandleType::NullHandle().GetIndex() )
                return nullptr;

            return &m_pComponentStorage->Get< ComponentType >( index );
        }

        void *GetRaw( EntityHandleType entityHandle )
        {
            const IndexType index = m_Pool.GetIndex( entityHandle.GetIndex() );
            return m_pComponentStorage->GetRaw( index );
        }

        const void *GetRaw( EntityHandleType entityHandle ) const
        {
            const IndexType index = m_Pool.GetIndex( entityHandle.GetIndex() );
            return m_pComponentStorage->GetRaw( index );
        }

        void Clear()
        {
            m_pComponentStorage->Clear();
            m_Pool.Clear();
        }

        bool Contains( EntityHandleType entityHandle ) const
        {
            return m_Pool.Contains( entityHandle.GetIndex() );
        }

        void Swap( IndexType lhs, IndexType rhs )
        {
            m_Pool.Swap( m_Pool.GetElement( lhs ), m_Pool.GetElement( rhs ) );
            m_pComponentStorage->Swap( lhs, rhs );
        }

        Uint32 GetItemCount() const
        {
            return m_Pool.GetItemCount();
        }

        IndexType GetIndex( EntityHandleType entityHandle ) const
        {
            return m_Pool.GetIndex( entityHandle.GetIndex() );
        }

        ConstIterator begin() const
        {
            return m_Pool.begin();
        }

        ConstIterator end() const
        {
            return m_Pool.end();
        }

      public:
        SparseSetType m_Pool{};
        ComponentStorage *m_pComponentStorage;

        std::vector< CreateHandler > m_Create;
        std::vector< DestroyHandler > m_Destroy;
    };
}