/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics::utils
{
    physx::PxVec3 ConvertToPhysXVector( const DirectX::XMFLOAT3 &vector );
    physx::PxQuat ConvertToPhysXQuat( const DirectX::XMFLOAT4 &quat );
    physx::PxTransform ConvertToPhysXTransform( const DirectX::XMFLOAT4X4 &transform );

    DirectX::XMFLOAT3 ConvertToDirectXVector( const physx::PxVec3 &vector );
    DirectX::XMFLOAT4 ConvertToDirectXQuat( const physx::PxQuat &quat );

    physx::PxFilterFlags SmileSimulationFilterShader( physx::PxFilterObjectAttributes attribute0,
        physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attribute1,
        physx::PxFilterData filterData1,
        physx::PxPairFlags &pairFlags,
        const void *pConstantBlock,
        physx::PxU32 constantBlockSize );
}