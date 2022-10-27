#pragma once

namespace smile::ecs
{
    template < typename... Components >
    struct ComponentList
    {
        static constexpr Uint32 size = sizeof...( Components );
    };

    namespace list
    {
        namespace detail
        {
            template < typename T >
            struct HeadImpl;

            template < typename H, typename... T >
            struct HeadImpl< ComponentList< H, T... > >
            {
                using type = H;
            };
        }

        template < typename List >
        using Head = typename detail::HeadImpl< List >::type;
    }
}