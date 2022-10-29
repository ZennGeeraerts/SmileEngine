#include "smile_engine/ecs/ecs_engine.h"

#include <thirdparty/catch.hpp>

using namespace Smile;

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

struct TestSystem final
{
    void onUpdate( ECS::ECSEngine& engine, Timestep delta_time )
    {
        int test = 0;
        //my_component.x += 10;
        //another_component.name = "modified by system";
    }

    //static ecs::ComponentList< MyComponent, AnotherComponent > components;
    //using components = MyComponent, AnotherComponent;
};

TEST_CASE( "ECS" )
{
    SECTION( "Entity" )
    {
        ECS::ECSEngine engine{};
        //engine.RegisterSystem< TestSystem >();

        ECS::EntityHandleType handle1 = engine.CreateEntity();
        ECS::EntityHandleType handle2 = engine.CreateEntity();
        ECS::EntityHandleType handle3 = engine.CreateEntity();

        engine.DestroyEntity( handle2 );
        engine.DestroyEntity( handle3 );

        ECS::EntityHandleType invalidHandle = ECS::g_NullHandle< Uint32 >;

        REQUIRE( engine.IsEntityActive( handle1 ) );
        REQUIRE( !engine.IsEntityActive( handle2 ) );
        REQUIRE( !engine.IsEntityActive( handle3 ) );
        REQUIRE( !engine.IsEntityActive( invalidHandle ) );

        ECS::EntityHandleType handle4 = engine.CreateEntity();
        ECS::EntityHandleType handle5 = engine.CreateEntity();
        ECS::EntityHandleType handle6 = engine.CreateEntity();
        ECS::EntityHandleType handle7 = engine.CreateEntity();

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

        //engine.onUpdate( 1.0f );

        // REQUIRE( my_component.x == 29u );
        // REQUIRE( another_component.name == "modified by system" );
    }

    SECTION( "View" )
    {
        ECS::ECSEngine engine{};

        ECS::EntityHandleType handle1 = engine.CreateEntity();
        ECS::EntityHandleType handle2 = engine.CreateEntity();
        ECS::EntityHandleType handle3 = engine.CreateEntity();

        engine.AddComponent< AnotherComponent >( handle1, "test" );
        engine.AddComponent< TestComponent >( handle1, 10, 2 );

        engine.AddComponent< AnotherComponent >( handle2, "this is a string" );
        //engine.addComponent< TestComponent >( handle2 );

        engine.AddComponent< AnotherComponent >( handle3, "name" );
        engine.AddComponent< TestComponent >( handle3, 5, -3 );

        for ( ECS::EntityHandleType entityHandle : engine.GetView< AnotherComponent, TestComponent >() )
        {
            const auto &[another, test] = engine.GetComponents< AnotherComponent, TestComponent >( entityHandle );
        }
    }

    SECTION( "group" )
    {
        ECS::ECSEngine engine{};

        ECS::EntityHandleType handle1 = engine.CreateEntity();
        ECS::EntityHandleType handle2 = engine.CreateEntity();
        ECS::EntityHandleType handle3 = engine.CreateEntity();

        engine.AddComponent< AnotherComponent >( handle1, "test" );
        engine.AddComponent< TestComponent >( handle1, 10, 2 );

        engine.AddComponent< AnotherComponent >( handle2, "this is a string" );
        // engine.addComponent< TestComponent >( handle2 );

        engine.AddComponent< AnotherComponent >( handle3, "name" );
        engine.AddComponent< TestComponent >( handle3, 5, -3 );

        std::vector< ECS::EntityHandleType > handles{};
        const auto &group = engine.GetGroup< AnotherComponent >( ECS::g_Get< TestComponent > );

        int i{};
        for ( ECS::EntityHandleType entityHandle : group )
        {
            const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
            ++i;
        }

        engine.AddComponent< TestComponent >( handle2, 7, 3 );
        for ( ECS::EntityHandleType entityHandle : group )
        {
            const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
            ++i;
        }

        engine.RemoveComponent< TestComponent >( handle3 );
        for ( ECS::EntityHandleType entityHandle : group )
        {
            const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
            ++i;
        }
    }

    SECTION( "cast" )
    {
        ECS::ECSEngine engine{};

        ECS::EntityHandleType handle1 = engine.CreateEntity();
        ECS::EntityHandleType handle2 = engine.CreateEntity();

        Uint32 val = handle1.Hash();
        Uint32 val2 = handle2.Hash();

        engine.Clear();
    }
}