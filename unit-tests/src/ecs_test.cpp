#include "smile_engine/ecs/ecs_engine.h"

#include <thirdparty/catch.hpp>

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

TEST_CASE( "ECS" )
{
    SECTION( "Entity" )
    {
        ecs::ECSEngine engine{};

        ecs::EntityHandle handle1 = engine.createEntity();
        ecs::EntityHandle handle2 = engine.createEntity();
        ecs::EntityHandle handle3 = engine.createEntity();

        engine.destroyEntity( handle2 );
        engine.destroyEntity( handle3 );

        ecs::EntityHandle invalid_handle = ecs::nullHandle;

        REQUIRE( engine.isEntityActive( handle1 ) );
        REQUIRE( !engine.isEntityActive( handle2 ) );
        REQUIRE( !engine.isEntityActive( handle3 ) );
        REQUIRE( !engine.isEntityActive( invalid_handle ) );

        ecs::EntityHandle handle4 = engine.createEntity();
        ecs::EntityHandle handle5 = engine.createEntity();
        ecs::EntityHandle handle6 = engine.createEntity();
        ecs::EntityHandle handle7 = engine.createEntity();

        REQUIRE( engine.isEntityActive( handle4 ) );
        REQUIRE( engine.isEntityActive( handle5 ) );
        REQUIRE( engine.isEntityActive( handle6 ) );

        engine.addComponent< TestComponent >( handle1, 12, 13 );
        auto &component = engine.getComponent< TestComponent >( handle1 );

        REQUIRE( component.x == 12 );
        REQUIRE( component.y == 13 );

        // engine.registerComponent< AnotherComponent >();
        engine.addComponent< TestComponent >( handle5, 167, 16 );
        engine.addComponent< AnotherComponent >( handle5, "test string" );
        engine.addComponent< MyComponent >( handle5, 16u, 86u, 10u );

        engine.addComponent< TestComponent >( handle4, 14, 15 );
        engine.addComponent< AnotherComponent >( handle4, "hello world" );
        engine.addComponent< MyComponent >( handle4, 1u, 2u, 3u );

        engine.addComponent< TestComponent >( handle6, 34, -42 );
        engine.addComponent< AnotherComponent >( handle6, "sample text" );
        engine.addComponent< MyComponent >( handle6, 19u, 12u, 982u );

        engine.addComponent< TestComponent >( handle7, -373, 3838 );
        engine.addComponent< AnotherComponent >( handle7, "just some words" );
        engine.addComponent< MyComponent >( handle7, 24u, 17u, 1678u );

        auto &component2 = engine.getComponent< TestComponent >( handle4 );
        auto &component3 = engine.getComponent< AnotherComponent >( handle4 );

        REQUIRE( component2.x == 14 );
        REQUIRE( component2.y == 15 );
        REQUIRE( component3.name == "hello world" );

        /*engine.removeComponent< TestComponent >( handle4 );
        auto& another_component = engine.getComponent< AnotherComponent >( handle4 );

        REQUIRE( another_component.name == "hello world" );*/

        engine.removeComponent< AnotherComponent >( handle4 );
        auto &test_component = engine.getComponent< TestComponent >( handle6 );
        auto &my_component = engine.getComponent< MyComponent >( handle6 );

        REQUIRE( test_component.x == 34 );
        REQUIRE( test_component.y == -42 );

        REQUIRE( my_component.x == 19u );
        REQUIRE( my_component.y == 12u );
        REQUIRE( my_component.z == 982u );

        /*auto &another_component = engine.getComponent< AnotherComponent >( handle4 );
        REQUIRE( another_component.name == "hello world" );*/

        auto &another_component = engine.getComponent< AnotherComponent >( handle5 );
        REQUIRE( another_component.name == "test string" );

        another_component = engine.getComponent< AnotherComponent >( handle6 );
        REQUIRE( another_component.name == "sample text" );

        another_component = engine.getComponent< AnotherComponent >( handle7 );
        REQUIRE( another_component.name == "just some words" );
    }
}