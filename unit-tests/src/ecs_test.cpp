#include "smile_engine/ecs/ecs_engine.h"
#include <catch/catch.hpp>

using namespace smile;

struct TestComponent final
{
    int x;
    int y;
};

struct AnotherComponent final
{
    std::string name;
};

struct MyComponent final
{
    unsigned int x;
    unsigned int y;
    unsigned int z;
};

struct DataComponent final
{
    double MyData;
};

TEST_CASE( "ECS" )
{
    SECTION( "Entity" )
    {
        ecs::ECSEngine engine{};
        // engine.RegisterSystem< TestSystem >();

        ecs::EntityHandleType handle1 = engine.CreateEntity();
        ecs::EntityHandleType handle2 = engine.CreateEntity();
        ecs::EntityHandleType handle3 = engine.CreateEntity();

        engine.DestroyEntity( handle2 );
        engine.DestroyEntity( handle3 );

        ecs::EntityHandleType invalidHandle = ecs::EntityHandleType::NullHandle();

        REQUIRE( engine.IsEntityActive( handle1 ) );
        REQUIRE( !engine.IsEntityActive( handle2 ) );
        REQUIRE( !engine.IsEntityActive( handle3 ) );
        REQUIRE( !engine.IsEntityActive( invalidHandle ) );

        ecs::EntityHandleType handle4 = engine.CreateEntity();
        ecs::EntityHandleType handle5 = engine.CreateEntity();
        ecs::EntityHandleType handle6 = engine.CreateEntity();
        ecs::EntityHandleType handle7 = engine.CreateEntity();

        REQUIRE( engine.IsEntityActive( handle4 ) );
        REQUIRE( engine.IsEntityActive( handle5 ) );
        REQUIRE( engine.IsEntityActive( handle6 ) );

        engine.AddComponent< TestComponent >( handle1, 12, 13 );
        auto &component = engine.GetComponent< TestComponent >( handle1 );

        REQUIRE( component.x == 12 );
        REQUIRE( component.y == 13 );

        // engine.registerComponent< AnotherComponent >();
        engine.AddComponent< TestComponent >( handle5, 167, 16 );
        engine.AddComponent< AnotherComponent >( handle5, "test string" );
        engine.AddComponent< MyComponent >( handle5, 16u, 86u, 10u );

        engine.AddComponent< TestComponent >( handle4, 14, 15 );
        engine.AddComponent< AnotherComponent >( handle4, "hello world" );
        engine.AddComponent< MyComponent >( handle4, 1u, 2u, 3u );

        engine.AddComponent< TestComponent >( handle6, 34, -42 );
        engine.AddComponent< AnotherComponent >( handle6, "sample text" );
        engine.AddComponent< MyComponent >( handle6, 19u, 12u, 982u );

        engine.AddComponent< TestComponent >( handle7, -373, 3838 );
        engine.AddComponent< AnotherComponent >( handle7, "just some words" );
        engine.AddComponent< MyComponent >( handle7, 24u, 17u, 1678u );

        auto &component2 = engine.GetComponent< TestComponent >( handle4 );
        auto &component3 = engine.GetComponent< AnotherComponent >( handle4 );

        REQUIRE( component2.x == 14 );
        REQUIRE( component2.y == 15 );
        REQUIRE( component3.name == "hello world" );

        /*engine.removeComponent< TestComponent >( handle4 );
        auto& another_component = engine.getComponent< AnotherComponent >( handle4 );

        REQUIRE( another_component.name == "hello world" );*/

        engine.RemoveComponent< AnotherComponent >( handle4 );
        auto &testComponent = engine.GetComponent< TestComponent >( handle6 );
        auto &myComponent = engine.GetComponent< MyComponent >( handle6 );

        REQUIRE( testComponent.x == 34 );
        REQUIRE( testComponent.y == -42 );

        REQUIRE( myComponent.x == 19u );
        REQUIRE( myComponent.y == 12u );
        REQUIRE( myComponent.z == 982u );

        /*auto &another_component = engine.getComponent< AnotherComponent >( handle4 );
        REQUIRE( another_component.name == "hello world" );*/

        auto &another_component = engine.GetComponent< AnotherComponent >( handle5 );
        REQUIRE( another_component.name == "test string" );

        another_component = engine.GetComponent< AnotherComponent >( handle6 );
        REQUIRE( another_component.name == "sample text" );

        another_component = engine.GetComponent< AnotherComponent >( handle7 );
        REQUIRE( another_component.name == "just some words" );

        // engine.group< TestComponent, AnotherComponent, MyComponent >();

        // engine.onUpdate( 1.0f );

        // REQUIRE( my_component.x == 29u );
        // REQUIRE( another_component.name == "modified by system" );
    }

    SECTION( "View" )
    {
        ecs::ECSEngine engine{};

        ecs::EntityHandleType handle1 = engine.CreateEntity();
        ecs::EntityHandleType handle2 = engine.CreateEntity();
        ecs::EntityHandleType handle3 = engine.CreateEntity();

        engine.AddComponent< AnotherComponent >( handle1, "test" );
        engine.AddComponent< TestComponent >( handle1, 10, 2 );

        engine.AddComponent< AnotherComponent >( handle2, "this is a string" );
        // engine.addComponent< TestComponent >( handle2 );

        engine.AddComponent< AnotherComponent >( handle3, "name" );
        engine.AddComponent< TestComponent >( handle3, 5, -3 );

        for ( ecs::EntityHandleType entityHandle : engine.GetView< AnotherComponent, TestComponent >() )
        {
            const auto &[another, test] = engine.GetComponents< AnotherComponent, TestComponent >( entityHandle );
        }
    }

    SECTION( "Group" )
    {
        ecs::ECSEngine engine{};

        ecs::EntityHandleType handle1 = engine.CreateEntity();
        ecs::EntityHandleType handle2 = engine.CreateEntity();
        ecs::EntityHandleType handle3 = engine.CreateEntity();

        engine.AddComponent< AnotherComponent >( handle1, "test" );
        engine.AddComponent< TestComponent >( handle1, 10, 2 );

        engine.AddComponent< AnotherComponent >( handle2, "this is a string" );
        // engine.addComponent< TestComponent >( handle2 );

        engine.AddComponent< AnotherComponent >( handle3, "name" );
        engine.AddComponent< TestComponent >( handle3, 5, -3 );

        std::vector< ecs::EntityHandleType > handles{};
        auto group = engine.GetGroup< AnotherComponent >( ecs::g_Get< TestComponent > );

        int i{};
        for ( ecs::EntityHandleType entityHandle : group )
        {
            const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
            ++i;
        }

        engine.AddComponent< TestComponent >( handle2, 7, 3 );
        for ( ecs::EntityHandleType entityHandle : group )
        {
            const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
            ++i;
        }

        engine.RemoveComponent< TestComponent >( handle3 );
        for ( ecs::EntityHandleType entityHandle : group )
        {
            const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
            ++i;
        }
    }

    SECTION( "cast" )
    {
        ecs::ECSEngine engine{};

        ecs::EntityHandleType handle1 = engine.CreateEntity();
        ecs::EntityHandleType handle2 = engine.CreateEntity();

        Uint32 val = handle1.Hash();
        Uint32 val2 = handle2.Hash();

        engine.Clear();
    }

    SECTION( "IsComponentOwned" )
    {
        ecs::ECSEngine engine{};

        ecs::EntityHandleType handle = engine.CreateEntity();

        engine.AddComponent< AnotherComponent >( handle, "test" );
        engine.AddComponent< TestComponent >( handle, 10, 2 );
        engine.AddComponent< DataComponent >( handle, 27.03 );
        engine.AddComponent< MyComponent >( handle, 16u, 86u, 10u );

        REQUIRE( !engine.IsComponentOwned< AnotherComponent >() );
        REQUIRE( !engine.IsComponentOwned< TestComponent >() );
        REQUIRE( !engine.IsComponentOwned< DataComponent >() );
        REQUIRE( !engine.IsComponentOwned< MyComponent >() );

        auto group1 = engine.GetGroup< AnotherComponent, TestComponent >();

        REQUIRE( engine.IsComponentOwned< AnotherComponent >() );
        REQUIRE( engine.IsComponentOwned< TestComponent >() );
        REQUIRE( !engine.IsComponentOwned< DataComponent >() );
        REQUIRE( !engine.IsComponentOwned< MyComponent >() );

        auto group2 = engine.GetGroup< DataComponent >( ecs::g_Get< MyComponent > );

        REQUIRE( engine.IsComponentOwned< AnotherComponent >() );
        REQUIRE( engine.IsComponentOwned< TestComponent >() );
        REQUIRE( engine.IsComponentOwned< DataComponent >() );
        REQUIRE( !engine.IsComponentOwned< MyComponent >() );
    }

    SECTION( "SortComponent" )
    {
        ecs::ECSEngine engine{};

        ecs::EntityHandleType entity1 = engine.CreateEntity();
        ecs::EntityHandleType entity2 = engine.CreateEntity();
        ecs::EntityHandleType entity3 = engine.CreateEntity();
        ecs::EntityHandleType entity4 = engine.CreateEntity();

        engine.AddComponent< AnotherComponent >( entity1, "test" );
        engine.AddComponent< AnotherComponent >( entity2, "data" );
        engine.AddComponent< AnotherComponent >( entity3, "string" );
        engine.AddComponent< AnotherComponent >( entity4, "another" );

        std::vector< ecs::EntityHandleType > entities{};
        {
            auto view = engine.GetView< AnotherComponent >();
            for ( auto e : view )
            {
                entities.push_back( e );
            }
        }

        REQUIRE( entities[0] == entity1 );
        REQUIRE( entities[1] == entity2 );
        REQUIRE( entities[2] == entity3 );
        REQUIRE( entities[3] == entity4 );

        REQUIRE( engine.GetComponent< AnotherComponent >( entities[0] ).name == "test" );
        REQUIRE( engine.GetComponent< AnotherComponent >( entities[1] ).name == "data" );
        REQUIRE( engine.GetComponent< AnotherComponent >( entities[2] ).name == "string" );
        REQUIRE( engine.GetComponent< AnotherComponent >( entities[3] ).name == "another" );

        engine.SortComponent< AnotherComponent >(
            [&engine]( const ecs::EntityHandleType lhs, const ecs::EntityHandleType rhs )
            {
                const auto &lhsComp = engine.GetComponent< AnotherComponent >( lhs );
                const auto &rhsComp = engine.GetComponent< AnotherComponent >( rhs );
                return lhsComp.name < rhsComp.name;
            } );

        entities.clear();
        {
            auto view = engine.GetView< AnotherComponent >();
            for ( auto e : view )
            {
                entities.push_back( e );
            }
        }

        REQUIRE( entities[0] == entity4 );
        REQUIRE( entities[1] == entity2 );
        REQUIRE( entities[2] == entity3 );
        REQUIRE( entities[3] == entity1 );

        REQUIRE( engine.GetComponent< AnotherComponent >( entities[0] ).name == "another" );
        REQUIRE( engine.GetComponent< AnotherComponent >( entities[1] ).name == "data" );
        REQUIRE( engine.GetComponent< AnotherComponent >( entities[2] ).name == "string" );
        REQUIRE( engine.GetComponent< AnotherComponent >( entities[3] ).name == "test" );
    }
}