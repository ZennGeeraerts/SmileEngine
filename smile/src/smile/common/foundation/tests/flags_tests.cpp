/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/foundation/flags.h"

namespace smile::foundation
{
    enum class TestFlags
    {
        One = 0,
        Two,
        Three,
        Four
    };

    namespace flagstests
    {
        static_assert( Flags< TestFlags >{}.GetFlags() == 0 );
        static_assert( Flags< TestFlags >{ TestFlags::One }.GetFlags() == 1 );
        static_assert( Flags< TestFlags >{ TestFlags::Two }.GetFlags() == 2 );
        static_assert( Flags< TestFlags >{ TestFlags::Four }.GetFlags() == 8 );

        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.GetFlags() == 3 );
        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two, TestFlags::Three }.GetFlags() == 7 );

        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.Has( TestFlags::One ) );
        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.Has( TestFlags::Two ) );
        static_assert( !Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.Has( TestFlags::Three ) );
        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two, TestFlags::Three }.Has( TestFlags::One ) );
        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two, TestFlags::Three }.Has( TestFlags::Two ) );
        static_assert( Flags< TestFlags >{ TestFlags::One, TestFlags::Two, TestFlags::Three }.Has( TestFlags::Three ) );
        static_assert( !Flags< TestFlags >{ TestFlags::One, TestFlags::Two, TestFlags::Three }.Has( TestFlags::Four ) );

        static_assert(
            Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.HasAll( { TestFlags::One, TestFlags::Two } ) );
        static_assert(
            !Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.HasAll( { TestFlags::Four, TestFlags::Two } ) );
        static_assert(
            !Flags< TestFlags >{ TestFlags::One, TestFlags::Two }.HasAll( { TestFlags::One, TestFlags::Three } ) );
    }
}