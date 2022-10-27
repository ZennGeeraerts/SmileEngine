#pragma once

#include "smile_engine/core/core.h"
#include "component_list.h"

#include <type_traits>

namespace smile::ecs
{
    namespace detail
    {
        template < typename System >
        constexpr decltype( std::declval< System::components >(), true ) hasComponents( Uint32 )
        {
            return true;
        }

        template < typename System >
        constexpr bool hasComponents( ... )
        {
            return false;
        }

        template < typename System, typename... Args >
        constexpr decltype( std::declval< System >().onUpdate( std::declval< Args >()... ), true ) hasUpdate( Uint32 )
        {
            return true;
        }

        template < typename System, typename... Args >
        constexpr bool hasUpdate( ... )
        {
            return false;
        }

        template < typename System, typename... Args >
        constexpr decltype( std::declval< System >().onCreate( std::declval< Args >()... ), true ) hasCreate( Uint32 )
        {
            return true;
        }

        template < typename System, typename... Args >
        constexpr bool hasCreate( ... )
        {
            return false;
        }

        template < typename System, typename... Args >
        constexpr decltype( std::declval< System >().onDestroy( std::declval< Args >()... ), true ) hasDestroy( Uint32 )
        {
            return true;
        }

        template < typename Component, typename... Args >
        constexpr bool hasDestroy( ... )
        {
            return false;
        }

        template < typename System, typename Engine, typename Time, typename Components >
        struct hasECSUpdateHelper
        {
            static constexpr bool value = false;
        };

        template < typename System, typename Engine, typename Time, typename... Components >
        struct hasECSUpdateHelper< System, Engine, Time, ComponentList< Components... > >
        {
            static constexpr bool value = detail::hasUpdate< System, Engine &, Components &..., Time >( 0 );
        };

        template < typename System, typename Time, typename Components >
        struct hasComponentsUpdateHelper;

        template < typename System, typename Time, typename... Components >
        struct hasComponentsUpdateHelper< System, Time, ComponentList< Components... > >
        {
            static constexpr bool value = detail::hasUpdate< System, Components &..., Time >( 0 );
        };
    }

    template < typename System >
    constexpr bool hasComponents = detail::hasComponents< System >( 0 );

    template < typename Engine, typename System, typename Time >
    constexpr bool hasECSUpdate = detail::hasUpdate< System, Engine &, Time >( 0 );

    template < typename Engine, typename System, typename Components, typename Time >
    constexpr bool hasECSComponentUpdate = detail::hasECSUpdateHelper< System, Engine, Time, Components >::value;

    template < typename System, typename Components, typename Time >
    constexpr bool hasComponentUpdate = detail::hasComponentsUpdateHelper< System, Time, Components >::value;

    template < typename System, typename Time, typename Handle >
    constexpr bool hasCreate = detail::hasCreate< System, Handle, Time & >( 0 );

    template < typename System, typename Time >
    constexpr bool hasDestroy = detail::hasDestroy< System, Time & >( 0 );

    // namespace detail
    //{
    //    template < typename, typename T >
    //    struct has_update
    //    {
    //        static_assert( std::integral_constant< T, false >::value,
    //            "Second template parameter needs to be of function type." );
    //    };

    //    template < typename C, typename Ret, typename... Args >
    //    struct has_update< C, Ret( Args... ) >
    //    {
    //      private:
    //        template < typename T >
    //        static constexpr auto check( T * ) ->
    //            typename std::is_same< decltype( std::declval< T >().onUpdate( std::declval< Args >()... ) ), Ret
    //            >::type;

    //        template < typename >
    //        static constexpr std::false_type check( ... );

    //      public:
    //        typedef decltype( check< C >( 0 ) ) type;
    //        static constexpr bool value = type::value;
    //    };

    //    template < typename, typename T >
    //    struct has_destroy
    //    {
    //        static_assert( std::integral_constant< T, false >::value,
    //            "Second template parameter needs to be of function type." );
    //    };

    //    template < typename C, typename Ret, typename... Args >
    //    struct has_destroy< C, Ret( Args... ) >
    //    {
    //      private:
    //        template < typename T >
    //        static constexpr auto check( T * ) ->
    //            typename std::is_same< decltype( std::declval< T >().onDestroy( std::declval< Args >()... ) ), Ret
    //            >::type;

    //        template < typename >
    //        static constexpr std::false_type check( ... );

    //      public:
    //        typedef decltype( check< C >( 0 ) ) type;
    //        static constexpr bool value = type::value;
    //    };

    //    template < typename, typename T >
    //    struct has_create
    //    {
    //        static_assert( std::integral_constant< T, false >::value,
    //            "Second template parameter needs to be of function type." );
    //    };

    //    template < typename C, typename Ret, typename... Args >
    //    struct has_create< C, Ret( Args... ) >
    //    {
    //      private:
    //        template < typename T >
    //        static constexpr auto check( T * ) ->
    //            typename std::is_same< decltype( std::declval< T >().create( std::declval< Args >()... ) ), Ret
    //            >::type;

    //        template < typename >
    //        static constexpr std::false_type check( ... );

    //      public:
    //        typedef decltype( check< C >( 0 ) ) type;
    //        static constexpr bool value = type::value;
    //    };

    //    template < typename S, typename T, typename Cs >
    //    struct has_ct_update_helper;

    //    template < typename S, typename T, typename... Cs >
    //    struct has_ct_update_helper< S, T, ComponentList< Cs... > >
    //    {
    //        using type = detail::has_update< S, void( Cs &..., T ) >;
    //    };

    //    template < typename S, typename E, typename T, typename Cs >
    //    struct has_ect_update_helper;

    //    template < typename S, typename E, typename T, typename... Cs >
    //    struct has_ect_update_helper< S, E, T, ComponentList< Cs... > >
    //    {
    //        using type = detail::has_update< S, void( E &, Cs &..., T ) >;
    //    };
    //}

    // template < typename C >
    // struct hasComponents
    //{
    //  private:
    //    template < typename T >
    //    static std::true_type test( typename T::components * );
    //    template < typename >
    //    static std::false_type test( ... );

    //  public:
    //    typedef decltype( test< C >( 0 ) ) type;
    //    static constexpr bool value = type::value;
    //};

    // template < typename E, typename S, typename T >
    // using hasECSUpdate = detail::has_update< S, void( E &, T ) >;

    // template < typename S, typename T >
    // using hasDestroy = detail::has_destroy< S, void( T & ) >;

    // template < typename S, typename T, typename H >
    // using hasCreate = detail::has_create< S, void( H, T &) >;

    // template < typename S, typename Cs, typename T >
    // using hasComponentUpdate = typename detail::has_ct_update_helper< S, T, Cs >::type;

    // template < typename E, typename S, typename Cs, typename T >
    // using hasECSComponentUpdate = typename detail::has_ect_update_helper< S, E, T, Cs >::type;
}