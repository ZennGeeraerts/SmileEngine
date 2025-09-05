/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/core/math/math.h"
#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "Vector4" )
    {
        SECTION( "+" )
        {
            math::Vector4 vector1{ 1.0f, 2.0f, 3.0f, 4.0f };
            math::Vector4 vector2{ 3.0f, 4.0f, 5.0f, 6.0f };
            math::Vector4 result = vector1 + vector2;

            REQUIRE( result.x == 4.0f );
            REQUIRE( result.y == 6.0f );
            REQUIRE( result.z == 8.0f );
            REQUIRE( result.w == 10.0f );
        }

        SECTION( "+=" )
        {
            math::Vector4 vector1{ 1.0f, 2.0f, 3.0f, 4.0f };
            math::Vector4 vector2{ 3.0f, 4.0f, 5.0f, 6.0f };

            vector1 += vector2;

            REQUIRE( vector1.x == 4.0f );
            REQUIRE( vector1.y == 6.0f );
            REQUIRE( vector1.z == 8.0f );
            REQUIRE( vector1.w == 10.0f );
        }

        SECTION( "-" )
        {
            math::Vector4 vector1{ 1.0f, 2.0f, 3.0f, 4.0f };
            math::Vector4 vector2{ 3.0f, 4.0f, 5.0f, 6.0f };
            math::Vector4 result = vector1 - vector2;

            REQUIRE( result.x == -2.0f );
            REQUIRE( result.y == -2.0f );
            REQUIRE( result.z == -2.0f );
            REQUIRE( result.w == -2.0f );

            result = -result;

            REQUIRE( result.x == 2.0f );
            REQUIRE( result.y == 2.0f );
            REQUIRE( result.z == 2.0f );
            REQUIRE( result.w == 2.0f );
        }

        SECTION( "-=" )
        {
            math::Vector4 vector1{ 1.0f, 2.0f, 3.0f, 4.0f };
            math::Vector4 vector2{ 3.0f, 4.0f, 5.0f, 6.0f };

            vector1 -= vector2;

            REQUIRE( vector1.x == -2.0f );
            REQUIRE( vector1.y == -2.0f );
            REQUIRE( vector1.z == -2.0f );
            REQUIRE( vector1.w == -2.0f );
        }

        SECTION( "*" )
        {
            math::Vector4 vector1{ 1.0f, 2.0f, 3.0f, 4.0f };
            math::Vector4 vector2{ 3.0f, 4.0f, 5.0f, 6.0f };
            math::Vector4 result = vector1 * vector2;

            REQUIRE( result.x == 3.0f );
            REQUIRE( result.y == 8.0f );
            REQUIRE( result.z == 15.0f );
            REQUIRE( result.w == 24.0f );

            math::Vector4 another{ 5.0f, 6.0f, 7.0f, 8.0f };
            result = another * 3.0f;

            REQUIRE( result.x == 15.0f );
            REQUIRE( result.y == 18.0f );
            REQUIRE( result.z == 21.0f );
            REQUIRE( result.w == 24.0f );
        }

        SECTION( "*=" )
        {
            math::Vector4 vector1{ 1.0f, 2.0f, 3.0f, 4.0f };
            math::Vector4 vector2{ 3.0f, 4.0f, 5.0f, 6.0f };

            vector1 *= vector2;

            REQUIRE( vector1.x == 3.0f );
            REQUIRE( vector1.y == 8.0f );
            REQUIRE( vector1.z == 15.0f );
            REQUIRE( vector1.w == 24.0f );

            math::Vector4 another{ 2.0f, 4.0f, 8.0f, 10.0f };

            another *= 5.0f;

            REQUIRE( another.x == 10.0f );
            REQUIRE( another.y == 20.0f );
            REQUIRE( another.z == 40.0f );
            REQUIRE( another.w == 50.0f );
        }

        SECTION( "/" )
        {
            math::Vector4 vector1{ 18.0f, 20.0f, 25.0f, 35.0f };
            math::Vector4 vector2{ 2.0f, 4.0f, 5.0f, 7.0f };
            math::Vector4 result = vector1 / vector2;

            REQUIRE( result.x == 9.0f );
            REQUIRE( result.y == 5.0f );
            REQUIRE( result.z == 5.0f );
            REQUIRE( result.w == 5.0f );

            math::Vector4 another{ 6.0f, 18.0f, 21.0f, 30.0f };
            result = another / 3.0f;

            REQUIRE( result.x == 2.0f );
            REQUIRE( result.y == 6.0f );
            REQUIRE( result.z == 7.0f );
            REQUIRE( result.w == 10.0f );
        }

        SECTION( "/=" )
        {
            math::Vector4 vector1{ 18.0f, 20.0f, 25.0f, 35.0f };
            math::Vector4 vector2{ 2.0f, 4.0f, 5.0f, 7.0f };

            vector1 /= vector2;

            REQUIRE( vector1.x == 9.0f );
            REQUIRE( vector1.y == 5.0f );
            REQUIRE( vector1.z == 5.0f );
            REQUIRE( vector1.w == 5.0f );

            math::Vector4 another{ 6.0f, 18.0f, 21.0f, 30.0f };

            another /= 3.0f;

            REQUIRE( another.x == 2.0f );
            REQUIRE( another.y == 6.0f );
            REQUIRE( another.z == 7.0f );
            REQUIRE( another.w == 10.0f );
        }

        SECTION( "==" )
        {
            math::Vector4 vector1{ 18.0f, 20.0f, 25.0f, 30.0f };
            math::Vector4 vector2{ 18.0f, 20.0f, 25.0f, 30.0f };
            math::Vector4 vector3{ 5.0f, -2.0f, 4.0f, 3.0f };
            math::Vector4 vector4{ 18.0f, 5.0f, 4.0f, 4.0f };
            math::Vector4 vector5{ 3.0f, 20.0f, -4.0f, 5.0f };

            REQUIRE( vector1 == vector2 );
            REQUIRE( vector1 != vector3 );
            REQUIRE( vector2 != vector3 );
            REQUIRE( vector1 != vector4 );
            REQUIRE( vector1 != vector5 );
        }
    }
}