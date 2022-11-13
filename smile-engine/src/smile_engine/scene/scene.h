#pragma once

#include "smile_engine/core/uuid.h"
#include "smile_engine/core/timestep.h"
#include "smile_engine/graphic/camera/editor_camera.h"

#include "smile_engine/ecs/ecs_engine.h"

namespace smile::scene
{
    class Entity;

    class Scene final
    {
      public:
        Scene();
        ~Scene();

        Entity CreateEntity();
        Entity CreateEntity( const std::string &name );
        Entity CreateEntity( UUID uuid, const std::string &name );
        void DestroyEntity( Entity entity );

        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnSimulationStart();
        void OnSimulationStop();

        void OnUpdateRuntime( Timestep deltaTime );
        void OnUpdateSimulation( Timestep deltaTime, graphic::EditorCamera &editorCamera );
        void OnUpdateEditor( Timestep deltaTime, graphic::EditorCamera &editorCamera );
        void OnViewportResize( Uint32 width, Uint32 height );

        Entity GetPrimaryCameraEntity();

        static Ref< Scene > Copy( const Ref< Scene > &pScene );

        void DuplicateEntity( Entity entity );

      private:
        template < typename ComponentType >
        void OnComponentAdded( Entity entity, ComponentType &component );

      private:
        ecs::ECSEngine m_ECSEngine;
        Uint32 m_ViewportWidth = 0;
        Uint32 m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}
