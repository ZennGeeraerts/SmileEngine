/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/core/ecs/timed_system.h"
#include "smile_engine/core/ecs/ecs_types.h"
#include "smile_engine/common/primitive/uuid.h"

#include "smile_engine/physics/rigidbody.h"
#include "smile_engine/physics/character_controller.h"

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/graphic/camera/editor_camera.h"

namespace smile::ecs
{
    class ECSEngine;
}

namespace smile::physics::ecs
{
    class PhysicsSystem final : public smile::ecs::TimedSystem< PhysicsSystem >
    {
      public:
        PhysicsSystem() = default;
        ~PhysicsSystem() = default;

        void OnAdd( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnRemove( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnUpdate( primitive::Timestep deltaTime );

        void OnDebugRender( const graphic::Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        void OnDebugRender( const graphic::EditorCamera &editorCamera );

        Ref< Rigidbody > GetRigidbody( primitive::UUID entityID ) const
        {
            SM_ASSERT( m_RigidbodyMap.find( entityID ) != m_RigidbodyMap.end(),
                "PhysicsSystem::GetRigidbody > Entity has no rigidbody" );

            return m_RigidbodyMap[entityID];
        }
        Ref< CharacterController > GetCharacterController( primitive::UUID entityID ) const
        {
            SM_ASSERT( m_CharacterControllerMap.find( entityID ) != m_CharacterControllerMap.end(),
                "PhysicsSystem::GetRigidbody > Entity has no rigidbody" );

            return m_CharacterControllerMap[entityID];
        }

      private:
        Ref< PhysicsWorld > m_pPhysicsWorld;
        mutable std::unordered_map< primitive::UUID, Ref< Rigidbody > > m_RigidbodyMap;
        mutable std::unordered_map< primitive::UUID, Ref< CharacterController > > m_CharacterControllerMap;

        std::function< void( smile::ecs::EntityHandleType ) > m_AddRigidbodyToEntity;
        std::function< void( smile::ecs::EntityHandleType ) > m_RemoveRigidbodyFromEntity;

        std::function< void( smile::ecs::EntityHandleType ) > m_AddCharacterControllerToEntity;
        std::function< void( smile::ecs::EntityHandleType ) > m_RemoveCharacterControllerFromEntity;
    };
}