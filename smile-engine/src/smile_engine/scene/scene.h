#pragma once

#include "smile_engine/core/uuid.h"
#include "smile_engine/core/timestep.h"
#include "smile_engine/graphic/camera/editor_camera.h"

#include "smile_engine/ecs/ecs_engine.h"
//#include <thirdparty/entt/entt.hpp>

namespace smile::scene
{
    class Entity;

    class Scene final
    {
      public:
        Scene();
        ~Scene();

        Entity createEntity();
        Entity createEntity( const std::string &name );
        Entity createEntity( UUID uuid, const std::string &name );
        void destroyEntity( Entity entity );

        void onRuntimeStart();
        void onRuntimeStop();
        void onSimulationStart();
        void onSimulationStop();

        void onUpdateRuntime( Timestep delta_time );
        void onUpdateSimulation( Timestep delta_time, graphic::EditorCamera &editor_camera );
        void onUpdateEditor( Timestep delta_time, graphic::EditorCamera &editor_camera );
        void onViewportResize( Uint32 width, Uint32 height );

        Entity getPrimaryCameraEntity();

        static Ref< Scene > copy( const Ref< Scene > &scene );

        void duplicateEntity( Entity entity );

      private:
        template < typename ComponentType >
        void onComponentAdded( Entity entity, ComponentType &component );

      private:
        //entt::registry registry;
        ecs::ECSEngine ecsEngine;
        Uint32 viewportWidth = 0;
        Uint32 viewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}
