/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/platform/platform_detection.h"

#include <memory>

#ifdef SM_PLATFORM_WINDOWS
#    if SM_DYNAMIC_LINK
#        ifdef SM_BUILD_DLL
#            define SMILE_API __declspec( dllexport )
#        else
#            define SMILE_API __declspec( dllimport )
#        endif
#    else
#        define SMILE_API
#    endif
#else
#    error SmileEngine only supports Windows
#endif

#define BIT( x ) ( 1 << x )
#define SM_BIND_EVENT_FN( fn ) std::bind( &fn, this, std::placeholders::_1 )
#define SM_EXPAND_MACRO( x ) x
#define SM_STRINGIFY_MACRO( x ) #x

#define SAFE_DELETE( p )                                                                                               \
    if ( p )                                                                                                           \
    {                                                                                                                  \
        delete ( p );                                                                                                  \
        ( p ) = nullptr;                                                                                               \
    }
#define SAFE_RELEASE( r )                                                                                              \
    if ( r )                                                                                                           \
    {                                                                                                                  \
        r->Release();                                                                                                  \
        ( r ) = nullptr;                                                                                               \
    }

namespace smile
{
    template < typename Type >
    using Scope = std::unique_ptr< Type >;

    template < typename Type, typename... Args >
    constexpr Scope< Type > CreateScope( Args... args )
    {
        return std::make_unique< Type >( std::forward< Args >( args )... );
    }

    template < typename Type >
    using Ref = std::shared_ptr< Type >;

    template < typename Type, typename... Args >
    constexpr Ref< Type > CreateRef( Args... args )
    {
        return std::make_shared< Type >( std::forward< Args >( args )... );
    }

    using Uint64 = uint64_t;
    using Uint32 = uint32_t;
    using Uint16 = uint16_t;
    using Uint8 = uint8_t;

    using Int64 = int64_t;
    using Int32 = int32_t;
    using Int16 = int16_t;
    using Int8 = int8_t;

    using Byte = unsigned char;

    using VoidPointer = void *;

    template < int >
    struct VoidPointerType;

    template <>
    struct VoidPointerType< 4 >
    {
        using Type = Uint32;
    };

    template <>
    struct VoidPointerType< 8 >
    {
        using Type = Uint64;
    };

    using UintPointer = VoidPointerType< sizeof( VoidPointer ) >::Type;

    using Index = Uint32;
    using Count = Uint32;
    static constexpr Index s_InvalidIndex = std::numeric_limits< Index >::max();
    static constexpr Count s_InvalidCount = std::numeric_limits< Count >::max();
}

#include "smile/common/foundation/assert.h"