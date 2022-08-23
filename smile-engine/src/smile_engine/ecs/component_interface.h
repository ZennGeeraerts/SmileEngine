#pragma once

#include "sparse_set.h"
#include "component_storage.h"

#include <functional>

namespace smile::ecs
{
    using createHandler = std::function< void( EntityHandle, void * ) >;
    using destroyHandler = std::function< void( void * ) >;

    class ComponentInterface final
    {
      public:
        virtual ~ComponentInterface()
        {
            SAFE_DELETE( componentStorage );
        }

        template < typename ComponentType >
        ComponentType &get( EntityHandle entity_handle )
        {
            const Uint32 index = sparseSet.getIndex( entity_handle.index );

            SM_ASSERT( index != nullHandle.index, "ComponentInterface::get > Invalid index" );

            return componentStorage->get< ComponentType >( index );
        }

        template < typename ComponentType >
        const ComponentType &get( EntityHandle entity_handle ) const
        {
            const Uint32 index = sparseSet.getIndex( entity_handle.index );
            return componentStorage->get< ComponentType >( index );
        }

        void *getRaw( EntityHandle entity_handle )
        {
            const Uint32 index = sparseSet.getIndex( entity_handle.index );
            return componentStorage->getRaw( index );
        }

        const void *getRaw( EntityHandle entity_handle ) const
        {
            const Uint32 index = sparseSet.getIndex( entity_handle.index );
            return componentStorage->getRaw( index );
        }

      public:
        bool relational;
        SparseSet< Uint32 > sparseSet{};
        ComponentStorage *componentStorage;

        std::vector< createHandler > create;
        std::vector< destroyHandler > destroy;
    };
}