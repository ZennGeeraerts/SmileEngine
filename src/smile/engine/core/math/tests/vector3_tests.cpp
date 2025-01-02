#include "smile_engine.h"
#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "Vector3" )
    {
        SECTION( "+" )
        {
            math::Vector3 vector1{ 1.0f, 2.0f, 3.0f };
            math::Vector3 vector2{ 3.0f, 4.0f, 5.0f };
            math::Vector3 result = vector1 + vector2;

            REQUIRE( result.x == 4.0f );
            REQUIRE( result.y == 6.0f );
            REQUIRE( result.z == 8.0f );
        }

        SECTION( "+=" )
        {
            math::Vector3 vector1{ 1.0f, 2.0f, 3.0f };
            math::Vector3 vector2{ 3.0f, 4.0f, 5.0f };

            vector1 += vector2;

            REQUIRE( vector1.x == 4.0f );
            REQUIRE( vector1.y == 6.0f );
            REQUIRE( vector1.z == 8.0f );
        }

        SECTION( "-" )
        {
            math::Vector3 vector1{ 1.0f, 2.0f, 3.0f };
            math::Vector3 vector2{ 3.0f, 4.0f, 5.0f };
            math::Vector3 result = vector1 - vector2;

            REQUIRE( result.x == -2.0f );
            REQUIRE( result.y == -2.0f );
            REQUIRE( result.z == -2.0f );

            result = -result;

            REQUIRE( result.x == 2.0f );
            REQUIRE( result.y == 2.0f );
            REQUIRE( result.z == 2.0f );
        }

        SECTION( "-=" )
        {
            math::Vector3 vector1{ 1.0f, 2.0f, 3.0f };
            math::Vector3 vector2{ 3.0f, 4.0f, 5.0f };

            vector1 -= vector2;

            REQUIRE( vector1.x == -2.0f );
            REQUIRE( vector1.y == -2.0f );
            REQUIRE( vector1.z == -2.0f );
        }

        SECTION( "*" )
        {
            math::Vector3 vector1{ 1.0f, 2.0f, 3.0f };
            math::Vector3 vector2{ 3.0f, 4.0f, 5.0f };
            math::Vector3 result = vector1 * vector2;

            REQUIRE( result.x == 3.0f );
            REQUIRE( result.y == 8.0f );
            REQUIRE( result.z == 15.0f );

            math::Vector3 another{ 5.0f, 6.0f, 7.0f };
            result = another * 3.0f;

            REQUIRE( result.x == 15.0f );
            REQUIRE( result.y == 18.0f );
            REQUIRE( result.z == 21.0f );
        }

        SECTION( "*=" )
        {
            math::Vector3 vector1{ 1.0f, 2.0f, 3.0f };
            math::Vector3 vector2{ 3.0f, 4.0f, 5.0f };

            vector1 *= vector2;

            REQUIRE( vector1.x == 3.0f );
            REQUIRE( vector1.y == 8.0f );
            REQUIRE( vector1.z == 15.0f );

            math::Vector3 another{ 2.0f, 4.0f, 8.0f };

            another *= 5.0f;

            REQUIRE( another.x == 10.0f );
            REQUIRE( another.y == 20.0f );
            REQUIRE( another.z == 40.0f );
        }

        SECTION( "/" )
        {
            math::Vector3 vector1{ 18.0f, 20.0f, 25.0f };
            math::Vector3 vector2{ 2.0f, 4.0f, 5.0f };
            math::Vector3 result = vector1 / vector2;

            REQUIRE( result.x == 9.0f );
            REQUIRE( result.y == 5.0f );
            REQUIRE( result.z == 5.0f );

            math::Vector3 another{ 6.0f, 18.0f, 21.0f };
            result = another / 3.0f;

            REQUIRE( result.x == 2.0f );
            REQUIRE( result.y == 6.0f );
            REQUIRE( result.z == 7.0f );
        }

        SECTION( "/=" )
        {
            math::Vector3 vector1{ 18.0f, 20.0f, 25.0f };
            math::Vector3 vector2{ 2.0f, 4.0f, 5.0f };

            vector1 /= vector2;

            REQUIRE( vector1.x == 9.0f );
            REQUIRE( vector1.y == 5.0f );
            REQUIRE( vector1.z == 5.0f );

            math::Vector3 another{ 6.0f, 18.0f, 21.0f };

            another /= 3.0f;

            REQUIRE( another.x == 2.0f );
            REQUIRE( another.y == 6.0f );
            REQUIRE( another.z == 7.0f );
        }

        SECTION( "==" )
        {
            math::Vector3 vector1{ 18.0f, 20.0f, 25.0f };
            math::Vector3 vector2{ 18.0f, 20.0f, 25.0f };
            math::Vector3 vector3{ 5.0f, -2.0f, 4.0f };
            math::Vector3 vector4{ 18.0f, 5.0f, 4.0f };
            math::Vector3 vector5{ 3.0f, 20.0f, -4.0f };

            REQUIRE( vector1 == vector2 );
            REQUIRE( vector1 != vector3 );
            REQUIRE( vector2 != vector3 );
            REQUIRE( vector1 != vector4 );
            REQUIRE( vector1 != vector5 );
        }
    }
}