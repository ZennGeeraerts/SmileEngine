#include "smpch.h"
#include "ecs_engine.h"

namespace smile::ecs
{
    ECSEngine::~ECSEngine()
    {
        for ( auto component_interface : components )
            delete component_interface;
    }

    void ECSEngine::removeComponent( ComponentInterface *component_interface, EntityHandle entity_handle )
    {
        void *component_data = component_interface->getRaw( entity_handle );

        if ( component_data == nullptr )
            return;

        callDestructors( component_interface, component_data );

        const auto dead_index = component_interface->sparseSet.erase( entity_handle.index );
        component_interface->componentStorage->removeSwap( dead_index );

        // if ( component_interface->relational )
        // relational_rebuild( ci, dead_eindex );
    }

    void ECSEngine::callDestructors( ComponentInterface *component_interface, void *data )
    {
        for ( auto destructor : component_interface->destroy )
            destructor( data );
    }
}