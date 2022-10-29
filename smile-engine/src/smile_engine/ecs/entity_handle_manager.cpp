#include "smpch.h"
#include "entity_handle_manager.h"

namespace Smile::ECS
{
    EntityHandleType EntityHandleManager::CreateEntity()
    {
        if ( m_AvailableEntities > 0 )
        {
            // Recycle entity
            auto &handle = m_Entities[m_NextFreeEntityIndex];

            Uint32 new_handle_index = handle.Index;
            std::swap( m_NextFreeEntityIndex, new_handle_index );
            handle.Index = new_handle_index;

            --m_AvailableEntities;

            return handle;
        }
        else
        {
            // Create new entity
            EntityHandleType handle{ static_cast< IndexType >( m_Entities.size() ), 0 };
            m_Entities.push_back( handle );
            return handle;
        }
    }

    void EntityHandleManager::DestroyEntity( EntityHandleType entityHandle )
    {
        auto &handle = m_Entities[entityHandle.Index];

        SM_ASSERT( handle.Generation == entityHandle.Generation,
            "ECSEngine::DestroyEntity > Entity handle generation mismatch" );

        ++handle.Generation;

        Uint32 newHandleIndex = handle.Index;
        std::swap( m_NextFreeEntityIndex, newHandleIndex );
        handle.Index = newHandleIndex;

        ++m_AvailableEntities;
    }

    bool EntityHandleManager::IsEntityActive( EntityHandleType entityHandle ) const
    {
        if ( !entityHandle.IsValid() )
            return false;

        return entityHandle == m_Entities[entityHandle.Index];
    }
}