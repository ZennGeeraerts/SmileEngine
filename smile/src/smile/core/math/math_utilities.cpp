/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "math_utilities.h"

#include <random>

namespace smile
{
    std::mt19937 g_GlobalGenerator{
        static_cast< Uint32 >( std::chrono::system_clock::now().time_since_epoch().count() ) };
}

namespace smile::math
{
    float GenerateRandom( float min, float max ) noexcept
    {
        std::uniform_real_distribution< float > distribution{ min, max };
        return distribution( g_GlobalGenerator );
    }

    Int32 GenerateRandomInt( Int32 min, Int32 max ) noexcept
    {
        std::uniform_int_distribution< Int32 > distribution{ min, max };
        return distribution( g_GlobalGenerator );
    }

    DirectX::XMFLOAT3 QuaternionToEuler( const DirectX::XMFLOAT4 &q )
    {
        const float xx = q.x * q.x;
        const float yy = q.y * q.y;
        const float zz = q.z * q.z;

        const float m31 = 2.f * q.x * q.z + 2.f * q.y * q.w;
        const float m32 = 2.f * q.y * q.z - 2.f * q.x * q.w;
        const float m33 = 1.f - 2.f * xx - 2.f * yy;

        const float cy = sqrtf( m33 * m33 + m31 * m31 );
        const float cx = atan2f( -m32, cy );
        if ( cy > 16.f * FLT_EPSILON )
        {
            const float m12 = 2.f * q.x * q.y + 2.f * q.z * q.w;
            const float m22 = 1.f - 2.f * xx - 2.f * zz;

            return DirectX::XMFLOAT3{ cx, atan2f( m31, m33 ), atan2f( m12, m22 ) };
        }
        else
        {
            const float m11 = 1.f - 2.f * yy - 2.f * zz;
            const float m21 = 2.f * q.x * q.y - 2.f * q.z * q.w;

            return DirectX::XMFLOAT3{ cx, 0.f, atan2f( -m21, m11 ) };
        }
    }

    bool DecomposeMatrix( const DirectX::XMFLOAT4X4 &transform,
        DirectX::XMFLOAT3 &translation,
        DirectX::XMFLOAT3 &rotation,
        DirectX::XMFLOAT3 &scale )
    {
        DirectX::XMFLOAT4X4 localMatrix{ transform };

        // Normalize the matrix.
        if ( AreEqual( localMatrix._44, 0.f ) )
            return false;

        // First, isolate perspective.  This is the messiest.
        if ( AreEqual( localMatrix._14, 0.f ) || AreEqual( localMatrix._24, 0.f ) || AreEqual( localMatrix._34, 0.f ) )
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
        for ( Uint32 i = 0; i < 3; ++i )
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
        if ( !AreEqual( cos( rotation.y ), 0.f ) )
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