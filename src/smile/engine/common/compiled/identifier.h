/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <string_view>

namespace smile::compiled::identifier
{
    constexpr size_t g_InitialValue = 2166136261u;

    constexpr size_t GetHashCode( const char *text, const size_t code = g_InitialValue )
    {
        return *text ? GetHashCode( text + 1, static_cast< size_t >( code * 16777619ull ) ^ *text ) : code;
    }

    constexpr size_t GetHashCodeWithSize( const char *text, const int size, const size_t code = g_InitialValue )
    {
        return size > 0 ? GetHashCodeWithSize( text + 1, size - 1, static_cast< size_t >( code * 16777619ull ) ^ *text )
                        : code;
    }

    constexpr size_t GetHashCode( const std::string_view &text )
    {
        return GetHashCodeWithSize( text.data(), text.size() );
    }
}