/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::physics
{
	class Rigidbody
	{
      public:
        Rigidbody();
        virtual ~Rigidbody() = default;

        Rigidbody( const PhysicsActor & ) = delete;
        Rigidbody( PhysicsActor && ) = delete;
        Rigidbody &operator=( const PhysicsActor & ) = delete;
        Rigidbody &operator=( PhysicsActor && ) = delete;
	};
}