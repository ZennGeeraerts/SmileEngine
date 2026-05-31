/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "hash_code.h"
#include "constant_text.h"

namespace smile::foundation::identifier
{
    constexpr HashCode g_InitialValue = 2166136261u;

    constexpr HashCode GetHashCode( const char *text, const HashCode code = g_InitialValue )
    {
        return *text ? GetHashCode( text + 1, static_cast< HashCode >( code * 16777619ull ) ^ *text ) : code;
    }

    constexpr HashCode GetHashCodeWithSize( const char *text, const Count size, const HashCode code = g_InitialValue )
    {
        return size > 0
                   ? GetHashCodeWithSize( text + 1, size - 1, static_cast< HashCode >( code * 16777619ull ) ^ *text )
                   : code;
    }

    constexpr HashCode GetHashCode( const ConstantText text )
    {
        return GetHashCodeWithSize( text.GetText(), text.GetCharCount() );
    }
}

namespace smile
{
    constexpr foundation::HashCode operator""_id( const char *text, const std::size_t )
    {
        return foundation::identifier::GetHashCode( text );
    }
}