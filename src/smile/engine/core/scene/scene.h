/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/primitive/uuid.h"
#include "engine/common/primitive/timestep.h"
#include "engine/graphic/camera/editor_camera.h"

#include "engine/core/ecs/ecs_engine.h"
#include "ecs/state/state_manager.h"

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
        Entity CreateEntity( primitive::UUID uuid, const std::string &name );
        void DestroyEntity( Entity entity );

        void OnOpen();
        void OnClose();

        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnSimulationStart();
        void OnSimulationStop();

        void OnUpdateRuntime( primitive::Timestep deltaTime );
        void OnUpdateSimulation( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera );
        void OnUpdateEditor( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera );
        void OnViewportResize( Uint32 width, Uint32 height );

        Entity GetPrimaryCameraEntity();

        static Ref< Scene > Copy( const Ref< Scene > &pScene );

        void DuplicateEntity( Entity entity );

        Entity GetEntityByUUID( primitive::UUID uuid );

        void AddForce( primitive::UUID entityID, const DirectX::XMFLOAT3 &force, bool autoAwake );
        void MoveCharacterController( primitive::UUID entityID, const DirectX::XMFLOAT3 &displacement, float minDist );

      private:
        template < typename ComponentType >
        void OnComponentAdded( Entity entity, ComponentType &component );

      private:
        smile::ecs::ECSEngine m_ECSEngine;
        Uint32 m_ViewportWidth = 0;
        Uint32 m_ViewportHeight = 0;

        std::unordered_map< primitive::UUID, smile::ecs::EntityHandleType > m_EntityMap{};

        smile::ecs::state::StateManager m_StateManager;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}
