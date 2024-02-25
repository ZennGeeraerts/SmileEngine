#pragma once

namespace smile::physics
{
    enum class CollisionGroupFlag : Uint32
    {
        Group0 = BIT( 0 ),
        Group1 = BIT( 1 ),
        Group2 = BIT( 2 ),
        Group3 = BIT( 3 ),
        Group4 = BIT( 4 ),
        Group5 = BIT( 5 ),
        Group6 = BIT( 6 ),
        Group7 = BIT( 7 ),
        Group8 = BIT( 8 ),
        Group9 = BIT( 9 )
    };
}