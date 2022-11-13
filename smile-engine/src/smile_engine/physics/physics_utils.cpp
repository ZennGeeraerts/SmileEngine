#include "smpch.h"
#include "physics_utils.h"

namespace smile::physics::utils
{
    physx::PxVec3 ConvertToPhysXVector( const DirectX::XMFLOAT3 &vector )
    {
        return *( physx::PxVec3 * )( &vector );
    }

    physx::PxQuat ConvertToPhysxQuat( const DirectX::XMFLOAT4 &quat )
    {
        return *( physx::PxQuat * )( &quat );
    }

    physx::PxTransform ConvertToPhysXTransform( const DirectX::XMFLOAT4X4 &transform )
    {
        DirectX::XMMATRIX transformMat = DirectX::XMLoadFloat4x4( &transform );
        DirectX::XMVECTOR scaleVec{};
        DirectX::XMVECTOR rotationVec{};
        DirectX::XMVECTOR positionVec{};
        DirectX::XMMatrixDecompose( &scaleVec, &rotationVec, &positionVec, transformMat );

        DirectX::XMFLOAT3 position{};
        DirectX::XMStoreFloat3( &position, positionVec );

        DirectX::XMFLOAT4 rotation{};
        DirectX::XMStoreFloat4( &rotation, rotationVec );

        return physx::PxTransform{ ConvertToPhysXVector( position ), ConvertToPhysxQuat( rotation ) };
    }

    DirectX::XMFLOAT3 ConvertToDirectXVector( const physx::PxVec3 &vector )
    {
        return *( DirectX::XMFLOAT3 * )( &vector );
    }

    DirectX::XMFLOAT4 ConvertToDirectXQuat( const physx::PxQuat &quat )
    {
        return *( DirectX::XMFLOAT4 * )( &quat );
    }

    physx::PxFilterFlags SmileSimulationFilterShader( physx::PxFilterObjectAttributes attribute0,
        physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attribute1,
        physx::PxFilterData filterData1,
        physx::PxPairFlags &pairFlags,
        const void *pConstantBlock,
        physx::PxU32 constantBlockSize )
    {
        if ( physx::PxFilterObjectIsTrigger( attribute0 ) || physx::PxFilterObjectIsTrigger( attribute1 ) )
        {
            pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
            return physx::PxFilterFlag::eDEFAULT;
        }

        pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
        pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

        if ( ( filterData0.word0 & filterData1.word1 ) || ( filterData1.word0 & filterData0.word1 ) )
        {
            pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
            pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
            pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
            return physx::PxFilterFlag::eDEFAULT;
        }

        return physx::PxFilterFlag::eSUPPRESS;
    }
}