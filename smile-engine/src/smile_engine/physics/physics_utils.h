#pragma once

#include <PxPhysicsAPI.h>

namespace smile::physics::utils
{
    physx::PxVec3 convertToPhysXVector( const DirectX::XMFLOAT3 &vector );
    physx::PxQuat convertToPhysxQuat( const DirectX::XMFLOAT4 &quat );
    physx::PxTransform convertToPhysXTransform( const DirectX::XMFLOAT4X4 &transform );

    DirectX::XMFLOAT3 convertToDirectXVector( const physx::PxVec3 &vector );
    DirectX::XMFLOAT4 convertToDirectXQuat( const physx::PxQuat &quat );

    physx::PxFilterFlags smileSimulationFilterShader( physx::PxFilterObjectAttributes attribute0,
        physx::PxFilterData filter_data0,
        physx::PxFilterObjectAttributes attribute1,
        physx::PxFilterData filter_data1,
        physx::PxPairFlags &pair_flags,
        const void *constant_block,
        physx::PxU32 constant_block_size );
}