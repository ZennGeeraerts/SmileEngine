/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "foundation/compiled.h"
#include "foundation/constant_text.h"

namespace smile::foundation
{
    namespace constanttexttest
    {
        static_assert( ConstantText{ "Test" }.GetCharCount() == 4 );
        static_assert( ConstantText{ "Test" } == "Test" );
        static_assert( ConstantText{ "Test", sizeof( "Test" ) - 1 } == "Test" );
        static_assert( ConstantText{ "Test", 2 } == "Te" );

        static_assert( ConstantText{ "Test" }.Skip( 0 ) == "Test" );
        static_assert( ConstantText{ "Test" }.Skip( 1 ) == "est" );
        static_assert( ConstantText{ "Test" }.Skip( 2 ) == "st" );
        static_assert( ConstantText{ "Test" }.Skip( 3 ) == "t" );
        static_assert( ConstantText{ "Test" }.Skip( 4 ) == "" );

        static_assert( ConstantText{ "Test" }.Shrink( 0 ) == "Test" );
        static_assert( ConstantText{ "Test" }.Shrink( 1 ) == "Tes" );
        static_assert( ConstantText{ "Test" }.Shrink( 2 ) == "Te" );
        static_assert( ConstantText{ "Test" }.Shrink( 3 ) == "T" );
        static_assert( ConstantText{ "Test" }.Shrink( 4 ) == "" );

        static_assert( !ConstantText{ "Test" }.IsEmpty() );
        static_assert( !ConstantText{ "T" }.IsEmpty() );
        static_assert( ConstantText{ "" }.IsEmpty() );
        static_assert( ConstantText{}.IsEmpty() );

        static_assert( ConstantText{ "Test" } == ConstantText{ "Test" } );
        static_assert( !( ConstantText{ "Test" } == ConstantText{ "Tes" } ) );
        static_assert( !( ConstantText{ "Test" } == ConstantText{ "" } ) );
        static_assert( ConstantText{ "" } == ConstantText{ "" } );
        static_assert( ConstantText{ "" } == ConstantText{} );

        static_assert( ConstantText{ "class MyClass" }.StartsWith( "class " ) );
        static_assert( ConstantText{ "class MyClass" }.StartsWith( "" ) );
        static_assert( !ConstantText{ "class MyClass" }.StartsWith( "struct " ) );
        static_assert( !ConstantText{ "class MyClass" }.StartsWith( "clasz" ) );
    }
}
