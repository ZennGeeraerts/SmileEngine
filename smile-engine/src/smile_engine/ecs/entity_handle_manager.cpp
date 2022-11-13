#include "smpch.h"
#include "entity_handle_manager.h"

namespace smile::ecs
{
    EntityHandleType EntityHandleManager::CreateEntity()
    {
        if ( m_AvailableEntities > 0 )
        {
            // Recycle entity
            auto &handle = m_Entities[m_NextFreeEntityIndex];

            Uint32 newHandleIndex = handle.m_Index;
            std::swap( m_NextFreeEntityIndex, newHandleIndex );
            handle.m_Index = newHandleIndex;

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
        auto &handle = m_Entities[entityHandle.m_Index];

        SM_ASSERT( handle.m_Generation == entityHandle.m_Generation,
            "ECSEngine::DestroyEntity > Entity handle generation mismatch" );

        ++handle.m_Generation;

        Uint32 newHandleIndex = handle.m_Index;
        std::swap( m_NextFreeEntityIndex, newHandleIndex );
        handle.m_Index = newHandleIndex;

        ++m_AvailableEntities;
    }

    bool EntityHandleManager::IsEntityActive( EntityHandleType entityHandle ) const
    {
        if ( !entityHandle.IsValid() )
            return false;

        return entityHandle == m_Entities[entityHandle.m_Index];
    }
}