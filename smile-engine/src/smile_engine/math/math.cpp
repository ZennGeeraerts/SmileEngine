#include "smpch.h"
#include "math.h"

#include <random>

namespace smile
{
    std::mt19937 globalGenerator{
        static_cast< Uint32 >( std::chrono::system_clock::now().time_since_epoch().count() ) };
}

namespace smile::math
{
    float generateRandom( float min, float max )
    {
        std::uniform_real_distribution< float > distribution{ min, max };
        return distribution( globalGenerator );
    }

    Int32 generateRandomInt( Int32 min, Int32 max )
    {
        std::uniform_int_distribution< Int32 > distribution{ min, max };
        return distribution( globalGenerator );
    }

    DirectX::XMFLOAT3 quaternionToEuler( const DirectX::XMFLOAT4 &quaternion )
    {
        DirectX::XMFLOAT3 euler{};

        // roll (x-axis rotation)
        double sinr_cosp = 2 * ( quaternion.w * quaternion.x + quaternion.y * quaternion.z );
        double cosr_cosp = 1 - 2 * ( quaternion.x * quaternion.x + quaternion.y * quaternion.y );
        euler.x = static_cast< float >( std::atan2( sinr_cosp, cosr_cosp ) );

        // pitch (y-axis rotation)
        float sinp = 2 * ( quaternion.w * quaternion.y - quaternion.z * quaternion.x );
        if ( std::abs( sinp ) >= 1 )
            euler.y = std::copysign( piDiv2, sinp ); // use 90 degrees if out of range
        else
            euler.y = std::asin( sinp );

        // yaw (z-axis rotation)
        float siny_cosp = 2 * ( quaternion.w * quaternion.z + quaternion.x * quaternion.y );
        float cosy_cosp = 1 - 2 * ( quaternion.y * quaternion.y + quaternion.z * quaternion.z );
        euler.z = std::atan2( siny_cosp, cosy_cosp );

        return euler;
    }

    bool decomposeMatrix( const DirectX::XMFLOAT4X4 &transform,
        DirectX::XMFLOAT3 &translation,
        DirectX::XMFLOAT3 &rotation,
        DirectX::XMFLOAT3 &scale )
    {
        DirectX::XMFLOAT4X4 local_matrix{ transform };

        // Normalize the matrix.
        if ( compareFloats( local_matrix._44, 0.f ) )
            return false;

        // First, isolate perspective.  This is the messiest.
        if ( compareFloats( local_matrix._14, 0.f ) || compareFloats( local_matrix._24, 0.f ) ||
             compareFloats( local_matrix._34, 0.f ) )
        {
            // Clear the perspective partition
            local_matrix._14 = local_matrix._24 = local_matrix._34 = 0.f;
            local_matrix._44 = 1.f;
        }

        // Next take care of translation (easy).
        translation = DirectX::XMFLOAT3( local_matrix._41, local_matrix._42, local_matrix._43 );
        local_matrix._41 = 0;
        local_matrix._42 = 0;
        local_matrix._43 = 0;

        DirectX::XMFLOAT3 row[3]{};

        // Now get scale and shear.
        for ( Uint32 i = 0; i < 3; ++i )
        {
            row[i].x = local_matrix( i, 0 );
            row[i].y = local_matrix( i, 1 );
            row[i].z = local_matrix( i, 2 );
        }

        // Compute X scale factor and normalize first row.
        DirectX::XMVECTOR scale_x_vec = DirectX::XMVector3Length( DirectX::XMLoadFloat3( &row[0] ) );
        scale.x = scale_x_vec.m128_f32[0];
        DirectX::XMVECTOR normalized_scale_x_vec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &row[0] ) );
        DirectX::XMStoreFloat3( &row[0], normalized_scale_x_vec );

        DirectX::XMVECTOR scale_y_vec = DirectX::XMVector3Length( DirectX::XMLoadFloat3( &row[1] ) );
        scale.y = scale_y_vec.m128_f32[0];
        DirectX::XMVECTOR normalized_scale_y_vec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &row[1] ) );
        DirectX::XMStoreFloat3( &row[1], normalized_scale_y_vec );

        DirectX::XMVECTOR scale_z_vec = DirectX::XMVector3Length( DirectX::XMLoadFloat3( &row[2] ) );
        scale.z = scale_z_vec.m128_f32[0];
        DirectX::XMVECTOR normalized_scale_z_vec = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &row[2] ) );
        DirectX::XMStoreFloat3( &row[2], normalized_scale_z_vec );

        rotation.y = asin( -row[0].z );
        // rotation.y = atan2(-row[0].z, sqrt(pow(row[1].z, 2) + pow(row[2].z, 2)));
        if ( !compareFloats( cos( rotation.y ), 0.f ) )
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