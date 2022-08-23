#pragma once

#include <string_view>

namespace smile::compiled
{
    template < typename Type >
    constexpr std::string_view getFullTypeName()
    {
#if defined _MSC_VER
        return __FUNCSIG__;
#elif defined __clang__ || __GNU__ || __GNUC__
        return __PRETTY_FUNCTION__;
#else
#    error "Unsupported compiler"
#endif
    }

    template < typename Type >
    constexpr std::string_view getTypeName()
    {
        size_t prefix_len = getFullTypeName< void >().find( "void" );
        size_t multiple = getFullTypeName< void >().size() - getFullTypeName< int >().size();
        size_t dummy_len = getFullTypeName< void >().size() - 4 * multiple;
        size_t target_len = ( getFullTypeName< Type >().size() - dummy_len ) / multiple;
        std::string_view rv = getFullTypeName< Type >().substr( prefix_len, target_len );

        if ( rv.rfind( ' ' ) == rv.npos )
            return rv;

        return rv.substr( rv.rfind( ' ' ) + 1 );
    }
}

namespace smile
{
    template < typename Type >
    constexpr std::string_view typeNameOf()
    {
        return compiled::getTypeName< Type >();
    }
}