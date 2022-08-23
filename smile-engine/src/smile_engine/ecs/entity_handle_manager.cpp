#include "smpch.h"
#include "entity_handle_manager.h"

namespace smile::ecs
{
    EntityHandle EntityHandleManager::createEntity()
    {
        if ( availableEntities > 0 )
        {
            // Recycle entity
            auto &handle = entities[nextFreeEntityIndex];

            Uint32 new_handle_index = handle.index;
            std::swap( nextFreeEntityIndex, new_handle_index );
            handle.index = new_handle_index;

            --availableEntities;

            return handle;
        }
        else
        {
            // Create new entity
            EntityHandle handle{ static_cast< Uint32 >( entities.size() ), 0 };
            entities.push_back( handle );
            return handle;
        }
    }

    void EntityHandleManager::destroyEntity( EntityHandle entity_handle )
    {
        auto &handle = entities[entity_handle.index];

        SM_ASSERT( handle.generation == entity_handle.generation,
            "ECSEngine::destroyEntity > Entity handle generation mismatch" );

        ++handle.generation;

        Uint32 newHandleIndex = handle.index;
        std::swap( nextFreeEntityIndex, newHandleIndex );
        handle.index = newHandleIndex;

        ++availableEntities;
    }

    bool EntityHandleManager::isEntityActive( EntityHandle entity_handle ) const
    {
        if ( !entity_handle.isValid() )
            return false;

        return entity_handle == entities[entity_handle.index];
    }
}