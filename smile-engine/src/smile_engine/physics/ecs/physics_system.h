/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/core/ecs/system.h"
#include "smile_engine/common/primitive/uuid.h"

#include "smile_engine/physics/rigidbody.h"

namespace smile::physics::ecs
{
	class PhysicsSystem final : public smile::ecs::System
	{
      public:
        PhysicsSystem() = default;
        ~PhysicsSystem() = default;

        void OnUpdate( primitive::Timestep deltaTime ) override;

    private:
        std::unordered_map< primitive::UUID, Ref< Rigidbody > > m_RigidbodyMap;
        std::unordered_map< primitive::UUID, Ref< CharacterController > > s_CharacterControllerMap;
	};
}