#pragma once

#include <string_view>

namespace smile::compiled::identifier
{
    constexpr size_t initialValue = 2166136261u;

    constexpr size_t getHashCode( const char *text, const size_t code = initialValue )
    {
        return *text ? getHashCode( text + 1, static_cast< size_t >( code * 16777619ull ) ^ *text ) : code;
    }

    constexpr size_t getHashCodeWithSize( const char *text, const int size, const size_t code = initialValue )
    {
        return size > 0 ? getHashCodeWithSize( text + 1, size - 1, static_cast< size_t >( code * 16777619ull ) ^ *text )
                        : code;
    }

    constexpr size_t getHashCode( const std::string_view &text )
    {
        return getHashCodeWithSize( text.data(), text.size() );
    }
}