#include "smpch.h"
#include "Math.h"

#include "SmileEngine/Utils/Utils.h"

namespace smile::Math
{
    DirectX::XMFLOAT3 QuaternionToEuler( const DirectX::XMFLOAT4 &quaternion )
    {
        DirectX::XMFLOAT3 euler{};

        // roll (x-axis rotation)
        double sinr_cosp = 2 * ( quaternion.w * quaternion.x + quaternion.y * quaternion.z );
        double cosr_cosp = 1 - 2 * ( quaternion.x * quaternion.x + quaternion.y * quaternion.y );
        euler.x = static_cast< float >( std::atan2( sinr_cosp, cosr_cosp ) );

        // pitch (y-axis rotation)
        float sinp = 2 * ( quaternion.w * quaternion.y - quaternion.z * quaternion.x );
        if ( std::abs( sinp ) >= 1 )
            euler.y = std::copysign( DirectX::XM_PI / 2, sinp ); // use 90 degrees if out of range
        else
            euler.y = std::asin( sinp );

        // yaw (z-axis rotation)
        float siny_cosp = 2 * ( quaternion.w * quaternion.z + quaternion.x * quaternion.y );
        float cosy_cosp = 1 - 2 * ( quaternion.y * quaternion.y + quaternion.z * quaternion.z );
        euler.z = std::atan2( siny_cosp, cosy_cosp );

        return euler;
    }

    bool DecomposeMatrix( const DirectX::XMFLOAT4X4 &transform,
        DirectX::XMFLOAT3 &translation,
        DirectX::XMFLOAT3 &rotation,
        DirectX::XMFLOAT3 &scale )
    {
        DirectX::XMFLOAT4X4 localMatrix{ transform };

        // Normalize the matrix.
        if ( CompareFloats( localMatrix._44, 0.f ) )
            return false;

        // First, isolate perspective.  This is the messiest.
        if ( CompareFloats( localMatrix._14, 0.f ) || CompareFloats( localMatrix._24, 0.f ) ||
             CompareFloats( localMatrix._34, 0.f ) )
        {
            // Clear the perspective partition
            localMatrix._14 = localMatrix._24 = localMatrix._34 = 0.f;
            localMatrix._44 = 1.f;
        }

        // Next take care of translation (easy).
        translation = DirectX::XMFLOAT3( localMatrix._41, localMatrix._42, localMatrix._43 );
        localMatrix._41 = 0;
        localMatrix._42 = 0;
        localMatrix._43 = 0;

        DirectX::XMFLOAT3 row[3]{};

        // Now get scale and shear.
        for ( uint32_t i = 0; i < 3; ++i )
        {
            row[i].x = localMatrix( i, 0 );
            row[i].y = localMatrix( i, 1 );
            row[i].z = localMatrix( i, 2 );
        }

        // Compute X scale factor and normalize first row.
        DirectX::XMVECTOR scaleXVec = DirectX::XMVector3Length( DirectX::XMLoadFloat3( &row[0] ) );
        scale.x = scaleXVec.m128_f32[0];
        DirectX::XMVECTOR normalizedScaleXVec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &row[0] ) );
        DirectX::XMStoreFloat3( &row[0], normalizedScaleXVec );

        DirectX::XMVECTOR scaleYVec = DirectX::XMVector3Length( DirectX::XMLoadFloat3( &row[1] ) );
        scale.y = scaleYVec.m128_f32[0];
        DirectX::XMVECTOR normalizedScaleYVec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &row[1] ) );
        DirectX::XMStoreFloat3( &row[1], normalizedScaleYVec );

        DirectX::XMVECTOR scaleZVec = DirectX::XMVector3Length( DirectX::XMLoadFloat3( &row[2] ) );
        scale.z = scaleZVec.m128_f32[0];
        DirectX::XMVECTOR normalizedScaleZVec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &row[2] ) );
        DirectX::XMStoreFloat3( &row[2], normalizedScaleZVec );

        rotation.y = asin( -row[0].z );
        // rotation.y = atan2(-row[0].z, sqrt(pow(row[1].z, 2) + pow(row[2].z, 2)));
        if ( !CompareFloats( cos( rotation.y ), 0.f ) )
        {
            rotation.x = atan2( row[1].z, row[2].z );
            rotation.z = atan2( row[0].y, row[0].x );
        }
        else
        {
            rotation.x = atan2( -row[2].x, row[1].y );
            rotation.z = 0;
        }

        return true;
    }
}