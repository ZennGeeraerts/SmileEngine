/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "hash_code.h"

#include <string_view>

namespace smile::foundation::identifier
{
    constexpr HashCode g_InitialValue = 2166136261u;

    constexpr HashCode GetHashCode( const char *text, const HashCode code = g_InitialValue )
    {
        return *text ? GetHashCode( text + 1, static_cast< HashCode >( code * 16777619ull ) ^ *text ) : code;
    }

    constexpr HashCode GetHashCodeWithSize( const char *text, const int size, const HashCode code = g_InitialValue )
    {
        return size > 0
                   ? GetHashCodeWithSize( text + 1, size - 1, static_cast< HashCode >( code * 16777619ull ) ^ *text )
                   : code;
    }

    constexpr HashCode GetHashCode( const std::string_view &text )
    {
        return GetHashCodeWithSize( text.data(), text.size() );
    }
}