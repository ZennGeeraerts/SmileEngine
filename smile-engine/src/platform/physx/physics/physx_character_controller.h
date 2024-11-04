/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/physics/character_controller.h"

namespace physx
{
    class PxController;
}

namespace smile::physics
{
    class PhysXCharacterController final : public CharacterController
    {
      public:
        PhysXCharacterController( physx::PxController *pController );
        ~PhysXCharacterController();

        void Translate( const DirectX::XMFLOAT3 &translation ) override;
        CollisionFlag Move( const DirectX::XMFLOAT3 &displacement, float minDist = 0 ) override;

        DirectX::XMFLOAT3 GetPosition() const override;
        DirectX::XMFLOAT3 GetFootPosition() const override;

      private:
        physx::PxController *m_pController;
    };
}