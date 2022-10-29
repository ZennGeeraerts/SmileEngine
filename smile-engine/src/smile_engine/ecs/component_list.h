#pragma once

namespace Smile::ECS
{
    template < typename... Components >
    struct ComponentList
    {
        static constexpr Uint32 Size = sizeof...( Components );
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
                using Type = H;
            };
        }

        template < typename List >
        using Head = typename detail::HeadImpl< List >::Type;
    }
}