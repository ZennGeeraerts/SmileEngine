#include "smpch.h"
#include "ecs_engine.h"

namespace Smile::ECS
{
    ECSEngine::~ECSEngine()
    {
        for ( auto pComponentInterface : m_pComponents )
            delete pComponentInterface;

       /* if ( !destructorHandlers.empty() )
        {
            for ( Uint32 i{ static_cast< Uint32 >( destructorHandlers.size() ) - 1 }; i > 0; --i )
                destructorHandlers[i]();

            destructorHandlers.clear();
        }

        updateHandlers.clear();*/
    }

    void ECSEngine::RemoveComponent( ComponentInterface *pComponentInterface, EntityHandleType entityHandle )
    {
        void *pComponentData = pComponentInterface->GetRaw( entityHandle );

        if ( pComponentData == nullptr )
            return;

        CallDestructors( pComponentInterface, pComponentData );

        for ( auto &group : m_Groups )
        {
            if ( group.HasComponent( pComponentInterface ) )
                group.RemoveEntity( entityHandle.Index );
        }

        const IndexType deadIndex = pComponentInterface->m_Pool.Erase( entityHandle.Index );
        pComponentInterface->m_pComponentStorage->RemoveSwap( deadIndex );

        // if ( component_interface->relational )
        // relational_rebuild( ci, dead_eindex );
    }

    void ECSEngine::CallDestructors( ComponentInterface *component_interface, void *data )
    {
        for ( auto destructor : component_interface->m_Destroy )
            destructor( data );
    }

    //void ECSEngine::onUpdate( Timestep delta_time )
    //{
    //    for ( auto update_handler : updateHandlers )
    //    {
    //        update_handler( delta_time );
    //    }
    //}

    void ECSEngine::Clear()
    {
        for ( auto pComponentInterface : m_pComponents )
        {
            for ( Uint32 i{}; i < pComponentInterface->m_pComponentStorage->GetSize(); ++i )
            {
                CallDestructors( pComponentInterface, pComponentInterface->m_pComponentStorage->GetRaw( i ) );
            }
            
            pComponentInterface->Clear();
        }

        m_Groups.clear();
    }
}