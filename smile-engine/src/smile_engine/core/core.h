#pragma once

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

#ifdef SM_C_DEBUG
#    ifdef SM_PLATFORM_WINDOWS
#        define SM_DEBUGBREAK() __debugbreak()
#    else
#        error "Platform doesn't support debugbreak"
#    endif
#    define SM_ENABLE_ASSERTS
#endif

#define BIT( x ) ( 1 << x )
#define SM_BIND_EVENT_FN( fn ) std::bind( &fn, this, std::placeholders::_1 )
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
    constexpr Scope< Type > createScope( Args... args )
    {
        return std::make_unique< Type >( std::forward< Args >( args )... );
    }

    template < typename Type >
    using Ref = std::shared_ptr< Type >;

    template < typename Type, typename... Args >
    constexpr Ref< Type > createRef( Args... args )
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
}

#include "logger.h"
#include "assert.h"