/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physics_utils.h"

namespace smile::physics::utils
{
    physx::PxVec3 ConvertToPhysXVector( const DirectX::XMFLOAT3 &vector )
    {
        return *( physx::PxVec3 * )( &vector );
    }

    physx::PxQuat ConvertToPhysXQuat( const DirectX::XMFLOAT4 &quat )
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

        return physx::PxTransform{ ConvertToPhysXVector( position ), ConvertToPhysXQuat( rotation ) };
    }

    DirectX::XMFLOAT3 ConvertToDirectXVector( const physx::PxVec3 &vector )
    {
        return *( DirectX::XMFLOAT3 * )( &vector );
    }

    DirectX::XMFLOAT4 ConvertToDirectXQuat( const physx::PxQuat &quat )
    {
        return *( DirectX::XMFLOAT4 * )( &quat );
    }

    DirectX::XMFLOAT4 ConvertToDirectXColor( physx::PxU32 color )
    {
        switch ( color )
        {
            case 0xFF000000:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Black );
            case 0xFFFF0000:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Red );
            case 0xFF00FF00:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Green );
            case 0xFF0000FF:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Blue );
            case 0xFFFFFF00:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Yellow );
            case 0xFFFF00FF:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Magenta );
            case 0xFF00FFFF:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Cyan );
            case 0xFFFFFFFF:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::White );
            case 0xFF808080:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Gray );
            case 0x88880000:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkRed );
            case 0x88008800:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkGreen );
            case 0x88000088:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkBlue );
            default:
                return static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::Black );
        }
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