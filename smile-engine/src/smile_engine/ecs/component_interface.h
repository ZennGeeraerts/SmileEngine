#pragma once

#include "smile_engine/stl/sparse_set.h"
#include "component_storage.h"

#include <functional>

namespace smile::ecs
{
    using createHandler = std::function< void( EntityHandleType, void * ) >;
    using destroyHandler = std::function< void( void * ) >;

    class ComponentInterface final
    {
      public:
        virtual ~ComponentInterface()
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

      public:
        bool m_IsRelational;
        SparseSetType m_Pool{};
        ComponentStorage *m_pComponentStorage;

        std::vector< createHandler > m_Create;
        std::vector< destroyHandler > m_Destroy;
    };
}