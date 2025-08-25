/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "compiler.h"
#include "constant_text.h"

namespace smile::foundation
{
    template < typename Type >
    constexpr ConstantText GetFullTypeName()
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
    constexpr ConstantText GetTypeName()
    {
        Count prefixCount = GetFullTypeName< void >().Find( "void" );
        Count multiple = GetFullTypeName< void >().GetCharCount() - GetFullTypeName< int >().GetCharCount();
        Count dummyCount = GetFullTypeName< void >().GetCharCount() - 4 * multiple;
        Count targetCount = ( GetFullTypeName< Type >().GetCharCount() - dummyCount ) / multiple;
        ConstantText rv = GetFullTypeName< Type >().SubStr( prefixCount, targetCount );

        if constexpr ( ExcludeNamespace )
        {
            Index pos = rv.ReverseFind( ':' );
            rv = rv.SubStr( pos + 1 );
        }

        if ( rv.ReverseFind( ' ' ) == s_InvalidIndex )
            return rv;

        return rv.SubStr( rv.ReverseFind( ' ' ) + 1 );
    }

    template < typename Type, bool ExcludeNamespace = false >
    constexpr ConstantText TypeNameOf()
    {
        return GetTypeName< Type, ExcludeNamespace >();
    }
}