#include "smpch.h"
#include "ecs_engine.h"

namespace smile::ecs
{
    ECSEngine::~ECSEngine()
    {
        for ( auto component_interface : components )
            delete component_interface;

        if ( !destructorHandlers.empty() )
        {
            for ( Uint32 i{ static_cast< Uint32 >( destructorHandlers.size() ) - 1 }; i > 0; --i )
                destructorHandlers[i]();

            destructorHandlers.clear();
        }

        updateHandlers.clear();
    }

    void ECSEngine::removeComponent( ComponentInterface *component_interface, EntityHandleType entity_handle )
    {
        void *component_data = component_interface->getRaw( entity_handle );

        if ( component_data == nullptr )
            return;

        callDestructors( component_interface, component_data );

        for ( auto &group : groups )
        {
            if ( group.hasComponent( component_interface ) )
                group.remove( entity_handle.index );
        }

        const IndexType dead_index = component_interface->sparseSet.erase( entity_handle.index );
        component_interface->componentStorage->removeSwap( dead_index );

        // if ( component_interface->relational )
        // relational_rebuild( ci, dead_eindex );
    }

    void ECSEngine::callDestructors( ComponentInterface *component_interface, void *data )
    {
        for ( auto destructor : component_interface->destroy )
            destructor( data );
    }

    void ECSEngine::onUpdate( Timestep delta_time )
    {
        for ( auto update_handler : updateHandlers )
        {
            update_handler( delta_time );
        }
    }

    void ECSEngine::clear()
    {
        for ( auto component_interface : components )
        {
            for ( Uint32 i{}; i < component_interface->componentStorage->getSize(); ++i )
            {
                callDestructors( component_interface, component_interface->componentStorage->getRaw( i ) );
            }
            
            component_interface->clear();
        }

        groups.clear();
    }
}