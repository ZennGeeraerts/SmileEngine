#include <smile_engine.h>
#include <thirdparty/catch.hpp>

using namespace smile;

TEST_CASE( "Vector2" )
{
    SECTION( "+" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };
        math::Vector2 result = vector1 + vector2;

        REQUIRE( result.x == 4.0f );
        REQUIRE( result.y == 6.0f );
    }

    SECTION( "+=" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };

        vector1 += vector2;

        REQUIRE( vector1.x == 4.0f );
        REQUIRE( vector1.y == 6.0f );
    }

    SECTION( "-" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };
        math::Vector2 result = vector1 - vector2;

        REQUIRE( result.x == -2.0f );
        REQUIRE( result.y == -2.0f );

        result = -result;

        REQUIRE( result.x == 2.0f );
        REQUIRE( result.y == 2.0f );
    }

    SECTION( "-=" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };

        vector1 -= vector2;

        REQUIRE( vector1.x == -2.0f );
        REQUIRE( vector1.y == -2.0f );
    }

    SECTION( "*" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };
        math::Vector2 result = vector1 * vector2;

        REQUIRE( result.x == 3.0f );
        REQUIRE( result.y == 8.0f );

        math::Vector2 another{ 5.0f, 6.0f };
        result = another * 3.0f;

        REQUIRE( result.x == 15.0f );
        REQUIRE( result.y == 18.0f );
    }

    SECTION( "*=" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };

        vector1 *= vector2;

        REQUIRE( vector1.x == 3.0f );
        REQUIRE( vector1.y == 8.0f );

        math::Vector2 another{ 2.0f, 4.0f };

        another *= 5.0f;

        REQUIRE( another.x == 10.0f );
        REQUIRE( another.y == 20.0f );
    }

    SECTION( "/" )
    {
        math::Vector2 vector1{ 18.0f, 20.0f };
        math::Vector2 vector2{ 2.0f, 4.0f };
        math::Vector2 result = vector1 / vector2;

        REQUIRE( result.x == 9.0f );
        REQUIRE( result.y == 5.0f );

        math::Vector2 another{ 6.0f, 18.0f };
        result = another / 3.0f;

        REQUIRE( result.x == 2.0f );
        REQUIRE( result.y == 6.0f );
    }

    SECTION( "/=" )
    {
        math::Vector2 vector1{ 18.0f, 20.0f };
        math::Vector2 vector2{ 2.0f, 4.0f };

        vector1 /= vector2;

        REQUIRE( vector1.x == 9.0f );
        REQUIRE( vector1.y == 5.0f );

        math::Vector2 another{ 6.0f, 18.0f };

        another /= 3.0f;

        REQUIRE( another.x == 2.0f );
        REQUIRE( another.y == 6.0f );
    }

    SECTION( "==" )
    {
        math::Vector2 vector1{ 18.0f, 20.0f };
        math::Vector2 vector2{ 18.0f, 20.0f };
        math::Vector2 vector3{ 5.0f, -2.0f };
        math::Vector2 vector4{ 18.0f, 5.0f };
        math::Vector2 vector5{ 3.0f, 20.0f };

        REQUIRE( vector1 == vector2 );
        REQUIRE( vector1 != vector3 );
        REQUIRE( vector2 != vector3 );
        REQUIRE( vector1 != vector4 );
        REQUIRE( vector1 != vector5 );
    }

    SECTION( "DotProduct" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };
        math::Vector2 vector3{ -2.0f, 7.0f };
        float result = math::dotProduct( vector1, vector2 );

        REQUIRE( result == ( 1.0f * 3.0f + 2.0f * 4.0f ) );

        result = math::dotProduct( vector1, vector3 );

        REQUIRE( result == ( 1.0f * -2.0f + 2.0f * 7.0f ) );
    }

    SECTION( "DistanceSqr" )
    {
        math::Vector2 vector1{ 1.0f, 2.0f };
        math::Vector2 vector2{ 3.0f, 4.0f };
        float result = math::distanceSqr( vector1, vector2 );

        REQUIRE( result == 8.0f );
    }

    SECTION( "Distance" )
    {
        math::Vector2 vector1{ 1.0f, 0.0f };
        float result = math::distance( smile::math::Vector2::zero, vector1 );

        REQUIRE( result == 1.0f );

        math::Vector2 vector2{ 1.0f, 1.0f };
        result = math::distance( vector1, vector2 );

        REQUIRE( result == 1.0f );

        result = math::distance( vector2, vector1 );
        REQUIRE( result == 1.0f );
    }

    SECTION( "math::LengthSqr" )
    {
        math::Vector2 vector{ 5.0f, 2.0f };

        CHECK( math::lengthSqr( vector ) == Approx{ 29.f } );

        math::Vector2 vector2{ 1.0f, 1.0f };

        CHECK( math::lengthSqr( vector2 ) == Approx( 2.f ) );
    }

    SECTION( "Length" )
    {
        float length{};

        length = math::length( math::Vector2::zero );
        CHECK( Approx{ 0 } == length );

        length = math::length( math::Vector2::one );
        CHECK( Approx( sqrt( 2.0f ) ) == length );

        float x = 5.0f;
        float y = 10.0f;
        math::Vector2 other{ x, y };

        length = math::length( other );
        CHECK( Approx{ sqrt( pow( x, 2 ) + pow( y, 2 ) ) } == length );
    }
}
