/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/utils.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "primitive::String utilities", "[primitive][text]" )
    {
        SECTION( "ToInt" )
        {
            String _5{ "5" };
            String _55{ "55" };
            String _555{ "555" };
            String wrong{ "555ah" };
            String wrong2{ "ah" };
            String floating{ "55.5" };

            auto value = ToInt( _5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 5 );

            value = ToInt( _55 );
            CHECK( value.has_value() );
            CHECK( value.value() == 55 );

            value = ToInt( _555 );
            CHECK( value.has_value() );
            CHECK( value.value() == 555 );

            value = ToInt( wrong );
            CHECK( !value.has_value() );

            value = ToInt( wrong2 );
            CHECK( !value.has_value() );

            value = ToInt( floating );
            CHECK( !value.has_value() );
        }

        SECTION( "ToFloat" )
        {
            String _5_5{ "5.5" };
            String __5{ ".5" };
            String _5{ "5" };
            String _0_5{ "0.5" };
            String _55_55{ "55.55" };

            auto value = ToFloat( _5_5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 5.5f );

            value = ToFloat( __5 );
            CHECK( value.has_value() );
            CHECK( value.value() == .5f );

            value = ToFloat( _5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 5.f );

            value = ToFloat( _0_5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 0.5f );

            value = ToFloat( _55_55 );
            CHECK( value.has_value() );
            CHECK( value.value() == 55.55f );
        }
    }
}