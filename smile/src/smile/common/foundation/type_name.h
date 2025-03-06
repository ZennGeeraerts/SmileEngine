/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "compiler.h"

#include <string_view>

namespace smile::foundation
{
    template < typename Type >
    constexpr std::string_view GetFullTypeName()
    {
#if defined SM_COMPILER_MSVC
        return __FUNCSIG__;
#elif defined SM_COMPILER_CLANG || SM_COMPILER_GNU || SM_COMPILER_GNUC
        return __PRETTY_FUNCTION__;
#else
#    error "Unsupported compiler"
#endif
    }

    template < typename Type, bool ExcludeNamespace = false >
    constexpr std::string_view GetTypeName()
    {
        size_t prefixLen = GetFullTypeName< void >().find( "void" );
        size_t multiple = GetFullTypeName< void >().size() - GetFullTypeName< int >().size();
        size_t dummyLen = GetFullTypeName< void >().size() - 4 * multiple;
        size_t targetLen = ( GetFullTypeName< Type >().size() - dummyLen ) / multiple;
        std::string_view rv = GetFullTypeName< Type >().substr( prefixLen, targetLen );

        if constexpr ( ExcludeNamespace )
        {
            size_t pos = rv.find_last_of( ':' );
            rv = rv.substr( pos + 1 );
        }

        if ( rv.rfind( ' ' ) == rv.npos )
            return rv;

        return rv.substr( rv.rfind( ' ' ) + 1 );
    }

    template < typename Type, bool ExcludeNamespace = false >
    constexpr std::string_view TypeNameOf()
    {
        return GetTypeName< Type, ExcludeNamespace >();
    }
}