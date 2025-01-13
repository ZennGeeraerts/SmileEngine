#include "engine/core/math/math_utilities.h"
#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "Math" )
    {
        SECTION( "QuaternionToEuler" )
        {
            DirectX::XMFLOAT3 rotationEulerDegrees{ -45.f, -30.f, 90.f };
            DirectX::XMFLOAT3 rotationEulerRadians{ DirectX::XMConvertToRadians( rotationEulerDegrees.x ),
                DirectX::XMConvertToRadians( rotationEulerDegrees.y ),
                DirectX::XMConvertToRadians( rotationEulerDegrees.z ) };

            DirectX::XMFLOAT4 rotationQuaternion{};
            DirectX::XMVECTOR rotationQuaternionVec = DirectX::XMQuaternionRotationRollPitchYaw(
                rotationEulerRadians.x, rotationEulerRadians.y, rotationEulerRadians.z );
            DirectX::XMStoreFloat4( &rotationQuaternion, rotationQuaternionVec );

            DirectX::XMFLOAT3 convertedRotationEuler = math::QuaternionToEuler( rotationQuaternion );

            DirectX::XMFLOAT3 convertedRotationEulerDegrees{ DirectX::XMConvertToDegrees( convertedRotationEuler.x ),
                DirectX::XMConvertToDegrees( convertedRotationEuler.y ),
                DirectX::XMConvertToDegrees( convertedRotationEuler.z ) };

            REQUIRE( rotationEulerDegrees.x == Approx{ convertedRotationEulerDegrees.x } );
            REQUIRE( rotationEulerDegrees.y == Approx{ convertedRotationEulerDegrees.y } );
            REQUIRE( rotationEulerDegrees.z == Approx{ convertedRotationEulerDegrees.z } );
        }
    }
}