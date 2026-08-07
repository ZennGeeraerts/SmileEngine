#pragma once

namespace smile::ecs
{
    template < typename ComponentType >
    struct With final
    {
        using Component = ComponentType;
    };

    template < typename ComponentType >
    struct Without final
    {
        using Component = ComponentType;
    };

    template < typename ComponentType >
    struct Changed final
    {
        using Component = ComponentType;
    };

    template < typename ComponentType >
    struct QueryMatcher final
    {
        static bool Matches( const ECSEngine &ecsEngine, const EntityHandle entityHandle )
        {
            return ecsEngine.HasComponent< ComponentType >( entityHandle );
        }
    };

    template < typename ComponentType >
    struct QueryMatcher< With< ComponentType > > final
    {
        static bool Matches( const ECSEngine &ecsEngine, const EntityHandle entityHandle )
        {
            return ecsEngine.HasComponent< ComponentType >( entityHandle );
        }
    };

    template < typename ComponentType >
    struct QueryMatcher< Changed< ComponentType > > final
    {
        static bool Matches( const ECSEngine &ecsEngine, const EntityHandle entityHandle )
        {
            return ecsEngine.HasComponent< ComponentType >( entityHandle ) &&
                   ecsEngine.GetComponentPool< ComponentType >( entityHandle )->HasChanged( entityHandle );
        }
    };
}