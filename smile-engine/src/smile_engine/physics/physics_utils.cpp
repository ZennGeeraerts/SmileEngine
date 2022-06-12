#include "smpch.h"
#include "physics_utils.h"

namespace smile::physics::utils
{
    physx::PxVec3 convertToPhysXVector( const DirectX::XMFLOAT3 &vector )
    {
        return *( physx::PxVec3 * )( &vector );
    }

    physx::PxQuat convertToPhysxQuat( const DirectX::XMFLOAT4 &quat )
    {
        return *( physx::PxQuat * )( &quat );
    }

    physx::PxTransform convertToPhysXTransform( const DirectX::XMFLOAT4X4 &transform )
    {
        DirectX::XMMATRIX transform_mat = DirectX::XMLoadFloat4x4( &transform );
        DirectX::XMVECTOR scale_vec{};
        DirectX::XMVECTOR rotation_vec{};
        DirectX::XMVECTOR position_vec{};
        DirectX::XMMatrixDecompose( &scale_vec, &rotation_vec, &position_vec, transform_mat );

        DirectX::XMFLOAT3 position{};
        DirectX::XMStoreFloat3( &position, position_vec );

        DirectX::XMFLOAT4 rotation{};
        DirectX::XMStoreFloat4( &rotation, rotation_vec );

        return physx::PxTransform{ convertToPhysXVector( position ), convertToPhysxQuat( rotation ) };
    }

    DirectX::XMFLOAT3 convertToDirectXVector( const physx::PxVec3 &vector )
    {
        return *( DirectX::XMFLOAT3 * )( &vector );
    }

    DirectX::XMFLOAT4 convertToDirectXQuat( const physx::PxQuat &quat )
    {
        return *( DirectX::XMFLOAT4 * )( &quat );
    }

    physx::PxFilterFlags smileSimulationFilterShader( physx::PxFilterObjectAttributes attribute0,
        physx::PxFilterData filter_data0,
        physx::PxFilterObjectAttributes attribute1,
        physx::PxFilterData filter_data1,
        physx::PxPairFlags &pair_flags,
        const void *constant_block,
        physx::PxU32 constant_block_size )
    {
        if ( physx::PxFilterObjectIsTrigger( attribute0 ) || physx::PxFilterObjectIsTrigger( attribute1 ) )
        {
            pair_flags = physx::PxPairFlag::eTRIGGER_DEFAULT;
            return physx::PxFilterFlag::eDEFAULT;
        }

        pair_flags = physx::PxPairFlag::eCONTACT_DEFAULT;
        pair_flags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

        if ( ( filter_data0.word0 & filter_data1.word1 ) || ( filter_data1.word0 & filter_data0.word1 ) )
        {
            pair_flags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
            pair_flags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
            pair_flags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
            return physx::PxFilterFlag::eDEFAULT;
        }

        return physx::PxFilterFlag::eSUPPRESS;
    }
}