#include "smile/core/ecs/ecs_engine.h"
#include <catch/catch.hpp>

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

struct TestObject
{
    static int s_DestructorCount;

    int Value;

    TestObject( int v ) : Value{ v }
    {
    }

    ~TestObject()
    {
        ++s_DestructorCount;
    }
};

int TestObject::s_DestructorCount = 0;

namespace smile
{
    TEST_CASE( "ECS" )
    {
        SECTION( "Entity" )
        {
            ecs::ECSEngine engine{};
            // engine.RegisterSystem< TestSystem >();

            ecs::EntityHandle handle1 = engine.CreateEntity();
            ecs::EntityHandle handle2 = engine.CreateEntity();
            ecs::EntityHandle handle3 = engine.CreateEntity();

            engine.DestroyEntity( handle2 );
            engine.DestroyEntity( handle3 );

            ecs::EntityHandle invalidHandle = ecs::EntityHandle::NullHandle();

            REQUIRE( engine.IsEntityActive( handle1 ) );
            REQUIRE( !engine.IsEntityActive( handle2 ) );
            REQUIRE( !engine.IsEntityActive( handle3 ) );
            REQUIRE( !engine.IsEntityActive( invalidHandle ) );

            ecs::EntityHandle handle4 = engine.CreateEntity();
            ecs::EntityHandle handle5 = engine.CreateEntity();
            ecs::EntityHandle handle6 = engine.CreateEntity();
            ecs::EntityHandle handle7 = engine.CreateEntity();

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

            ecs::EntityHandle handle1 = engine.CreateEntity();
            ecs::EntityHandle handle2 = engine.CreateEntity();
            ecs::EntityHandle handle3 = engine.CreateEntity();

            engine.AddComponent< AnotherComponent >( handle1, "test" );
            engine.AddComponent< TestComponent >( handle1, 10, 2 );

            engine.AddComponent< AnotherComponent >( handle2, "this is a string" );
            // engine.addComponent< TestComponent >( handle2 );

            engine.AddComponent< AnotherComponent >( handle3, "name" );
            engine.AddComponent< TestComponent >( handle3, 5, -3 );

            for ( ecs::EntityHandle entityHandle : engine.GetView< AnotherComponent, TestComponent >() )
            {
                const auto &[another, test] = engine.GetComponents< AnotherComponent, TestComponent >( entityHandle );
            }
        }

        SECTION( "Group" )
        {
            ecs::ECSEngine engine{};

            ecs::EntityHandle handle1 = engine.CreateEntity();
            ecs::EntityHandle handle2 = engine.CreateEntity();
            ecs::EntityHandle handle3 = engine.CreateEntity();

            engine.AddComponent< AnotherComponent >( handle1, "test" );
            engine.AddComponent< TestComponent >( handle1, 10, 2 );

            engine.AddComponent< AnotherComponent >( handle2, "this is a string" );
            // engine.addComponent< TestComponent >( handle2 );

            engine.AddComponent< AnotherComponent >( handle3, "name" );
            engine.AddComponent< TestComponent >( handle3, 5, -3 );

            std::vector< ecs::EntityHandle > handles{};
            auto group = engine.GetGroup< AnotherComponent >( ecs::g_Get< TestComponent > );

            int i{};
            for ( ecs::EntityHandle entityHandle : group )
            {
                const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
                ++i;
            }

            engine.AddComponent< TestComponent >( handle2, 7, 3 );
            for ( ecs::EntityHandle entityHandle : group )
            {
                const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
                ++i;
            }

            engine.RemoveComponent< TestComponent >( handle3 );
            for ( ecs::EntityHandle entityHandle : group )
            {
                const auto &[test, another] = engine.GetComponents< TestComponent, AnotherComponent >( entityHandle );
                ++i;
            }
        }

        SECTION( "cast" )
        {
            ecs::ECSEngine engine{};

            ecs::EntityHandle handle1 = engine.CreateEntity();
            ecs::EntityHandle handle2 = engine.CreateEntity();

            Uint32 val = handle1.Hash();
            Uint32 val2 = handle2.Hash();

            engine.Clear();
        }

        SECTION( "IsComponentOwned" )
        {
            ecs::ECSEngine engine{};

            ecs::EntityHandle handle = engine.CreateEntity();

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

            ecs::EntityHandle entity1 = engine.CreateEntity();
            ecs::EntityHandle entity2 = engine.CreateEntity();
            ecs::EntityHandle entity3 = engine.CreateEntity();
            ecs::EntityHandle entity4 = engine.CreateEntity();

            engine.AddComponent< AnotherComponent >( entity1, "test" );
            engine.AddComponent< AnotherComponent >( entity2, "data" );
            engine.AddComponent< AnotherComponent >( entity3, "string" );
            engine.AddComponent< AnotherComponent >( entity4, "another" );

            std::vector< ecs::EntityHandle > entities{};
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
                [&engine]( const ecs::EntityHandle lhs, const ecs::EntityHandle rhs )
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

        SECTION( "On construction callback should execute after adding a component" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.OnConstruction< TestComponent >().emplace_back(
                []( ecs::ECSEngine &ecsEngine, ecs::EntityHandle entityHandle )
                {
                    TestComponent &component = ecsEngine.GetComponent< TestComponent >( entityHandle );
                    REQUIRE( component.x == 10 );
                    REQUIRE( component.y == 20 );
                } );

            engine.AddComponent< TestComponent >( entity, 10, 20 );
        }

        SECTION( "On destruction callback should execute after adding a component" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.OnDestruction< TestComponent >().emplace_back(
                []( ecs::ECSEngine &ecsEngine, ecs::EntityHandle entityHandle )
                {
                    TestComponent &component = ecsEngine.GetComponent< TestComponent >( entityHandle );
                    REQUIRE( component.x == 10 );
                    REQUIRE( component.y == 20 );
                } );

            engine.AddComponent< TestComponent >( entity, 10, 20 );
            engine.RemoveComponent< TestComponent >( entity );
        }

        SECTION( "Emplace, Get and Has work correctly", "[Context]" )
        {
            ecs::ECSEngine::Context ctx;

            REQUIRE_FALSE( ctx.Has< TestObject >() );

            auto &obj = ctx.Emplace< TestObject >( 42 );

            REQUIRE( ctx.Has< TestObject >() );
            REQUIRE( &obj == &ctx.Get< TestObject >() );
            REQUIRE( ctx.Get< TestObject >().Value == 42 );
        }

        SECTION( "Destructor is called on Erase", "[Context]" )
        {
            ecs::ECSEngine::Context ctx;

            TestObject::s_DestructorCount = 0;

            ctx.Emplace< TestObject >( 123 );

            REQUIRE( TestObject::s_DestructorCount == 0 );

            ctx.Erase< TestObject >();

            REQUIRE( TestObject::s_DestructorCount == 1 );
            REQUIRE_FALSE( ctx.Has< TestObject >() );
        }

        SECTION( "Destructor is called on Clear", "[Context]" )
        {
            ecs::ECSEngine::Context ctx;

            TestObject::s_DestructorCount = 0;

            ctx.Emplace< TestObject >( 1 );

            ctx.Clear();

            REQUIRE( TestObject::s_DestructorCount == 1 );
            REQUIRE_FALSE( ctx.Has< TestObject >() );
        }

        SECTION( "GetOrEmplace returns existing instance", "[Context]" )
        {
            ecs::ECSEngine::Context ctx;

            auto &obj1 = ctx.GetOrEmplace< TestObject >( 10 );
            auto &obj2 = ctx.GetOrEmplace< TestObject >( 999 );

            REQUIRE( &obj1 == &obj2 );
            REQUIRE( obj2.Value == 10 );
        }

        SECTION( "Erase on non-existing type is safe", "[Context]" )
        {
            ecs::ECSEngine::Context ctx;

            REQUIRE_NOTHROW( ctx.Erase< TestObject >() );
        }

        SECTION( "PatchComponent fires OnPatch listener", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            bool patchCalled = false;
            engine.OnPatch< TestComponent >().emplace_back(
                [&patchCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { patchCalled = true; } );

            engine.PatchComponent< TestComponent >( entity );

            REQUIRE( patchCalled );
        }

        SECTION( "PatchComponent passes correct entity handle to OnPatch listener", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 5, 10 );

            ecs::EntityHandle receivedHandle = ecs::EntityHandle::NullHandle();
            engine.OnPatch< TestComponent >().emplace_back(
                [&receivedHandle]( ecs::ECSEngine &, ecs::EntityHandle entityHandle )
                { receivedHandle = entityHandle; } );

            engine.PatchComponent< TestComponent >( entity );

            REQUIRE( receivedHandle == entity );
        }

        SECTION( "PatchComponent with modifier function updates component before firing signal", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            int valueInListener = 0;
            engine.OnPatch< TestComponent >().emplace_back(
                [&valueInListener]( ecs::ECSEngine &ecsEngine, ecs::EntityHandle entityHandle )
                { valueInListener = ecsEngine.GetComponent< TestComponent >( entityHandle ).x; } );

            engine.PatchComponent< TestComponent >( entity,
                []( ecs::ECSEngine &ecsEngine, ecs::EntityHandle entityHandle )
                { ecsEngine.GetComponent< TestComponent >( entityHandle ).x = 99; } );

            REQUIRE( engine.GetComponent< TestComponent >( entity ).x == 99 );
            REQUIRE( valueInListener == 99 );
        }

        SECTION( "PatchComponent returns reference to the component", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 7, 8 );

            TestComponent &comp = engine.PatchComponent< TestComponent >( entity );

            REQUIRE( comp.x == 7 );
            REQUIRE( comp.y == 8 );
            REQUIRE( &comp == &engine.GetComponent< TestComponent >( entity ) );
        }

        SECTION( "Multiple OnPatch listeners all fire in registration order", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            std::vector< int > callOrder{};
            engine.OnPatch< TestComponent >().emplace_back(
                [&callOrder]( ecs::ECSEngine &, ecs::EntityHandle ) { callOrder.push_back( 1 ); } );
            engine.OnPatch< TestComponent >().emplace_back(
                [&callOrder]( ecs::ECSEngine &, ecs::EntityHandle ) { callOrder.push_back( 2 ); } );
            engine.OnPatch< TestComponent >().emplace_back(
                [&callOrder]( ecs::ECSEngine &, ecs::EntityHandle ) { callOrder.push_back( 3 ); } );

            engine.PatchComponent< TestComponent >( entity );

            REQUIRE( callOrder.size() == 3 );
            REQUIRE( callOrder[0] == 1 );
            REQUIRE( callOrder[1] == 2 );
            REQUIRE( callOrder[2] == 3 );
        }

        SECTION( "PatchComponent does not fire construction or destruction signals", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            bool constructionCalled = false;
            bool destructionCalled = false;
            bool patchCalled = false;

            engine.OnConstruction< TestComponent >().emplace_back(
                [&constructionCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { constructionCalled = true; } );
            engine.OnDestruction< TestComponent >().emplace_back(
                [&destructionCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { destructionCalled = true; } );
            engine.OnPatch< TestComponent >().emplace_back(
                [&patchCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { patchCalled = true; } );

            engine.PatchComponent< TestComponent >( entity );

            REQUIRE_FALSE( constructionCalled );
            REQUIRE_FALSE( destructionCalled );
            REQUIRE( patchCalled );
        }

        SECTION( "AddOrReplaceComponent fires OnPatch when replacing an existing component", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            bool patchCalled = false;
            engine.OnPatch< TestComponent >().emplace_back(
                [&patchCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { patchCalled = true; } );

            engine.AddOrReplaceComponent< TestComponent >( entity, 10, 20 );

            REQUIRE( patchCalled );
        }

        SECTION( "AddOrReplaceComponent does not fire OnPatch when adding a fresh component", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();

            bool patchCalled = false;
            engine.OnPatch< TestComponent >().emplace_back(
                [&patchCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { patchCalled = true; } );

            engine.AddOrReplaceComponent< TestComponent >( entity, 10, 20 );

            REQUIRE_FALSE( patchCalled );
        }

        SECTION( "AddOrReplaceComponent updates component data when replacing", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.AddComponent< TestComponent >( entity, 1, 2 );
            engine.AddOrReplaceComponent< TestComponent >( entity, 42, 84 );

            const auto &comp = engine.GetComponent< TestComponent >( entity );
            REQUIRE( comp.x == 42 );
            REQUIRE( comp.y == 84 );
        }

        SECTION( "AddOrReplaceComponent fires OnConstruction when adding a fresh component", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();

            bool constructionCalled = false;
            engine.OnConstruction< TestComponent >().emplace_back(
                [&constructionCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { constructionCalled = true; } );

            engine.AddOrReplaceComponent< TestComponent >( entity, 10, 20 );

            REQUIRE( constructionCalled );
        }

        SECTION(
            "AddOrReplaceComponent does not fire OnConstruction when replacing an existing component", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            bool constructionCalled = false;
            engine.OnConstruction< TestComponent >().emplace_back(
                [&constructionCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { constructionCalled = true; } );

            engine.AddOrReplaceComponent< TestComponent >( entity, 10, 20 );

            REQUIRE_FALSE( constructionCalled );
        }

        SECTION( "ReplaceComponent updates component data", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.AddComponent< TestComponent >( entity, 1, 2 );
            engine.ReplaceComponent< TestComponent >( entity, 42, 84 );

            const auto &comp = engine.GetComponent< TestComponent >( entity );
            REQUIRE( comp.x == 42 );
            REQUIRE( comp.y == 84 );
        }

        SECTION( "ReplaceComponent fires OnPatch", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            bool patchCalled = false;
            engine.OnPatch< TestComponent >().emplace_back(
                [&patchCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { patchCalled = true; } );

            engine.ReplaceComponent< TestComponent >( entity, 10, 20 );

            REQUIRE( patchCalled );
        }

        SECTION( "ReplaceComponent does not fire OnConstruction", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 1, 2 );

            bool constructionCalled = false;
            engine.OnConstruction< TestComponent >().emplace_back(
                [&constructionCalled]( ecs::ECSEngine &, ecs::EntityHandle ) { constructionCalled = true; } );

            engine.ReplaceComponent< TestComponent >( entity, 10, 20 );

            REQUIRE_FALSE( constructionCalled );
        }

        SECTION( "OnPatch listener receives engine reference that can access component", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity, 55, 66 );

            int capturedX = 0;
            int capturedY = 0;
            engine.OnPatch< TestComponent >().emplace_back(
                [&capturedX, &capturedY]( ecs::ECSEngine &ecsEngine, ecs::EntityHandle entityHandle )
                {
                    const auto &comp = ecsEngine.GetComponent< TestComponent >( entityHandle );
                    capturedX = comp.x;
                    capturedY = comp.y;
                } );

            engine.PatchComponent< TestComponent >( entity );

            REQUIRE( capturedX == 55 );
            REQUIRE( capturedY == 66 );
        }

        SECTION( "PatchComponent fires listener only for patched entity", "[ECSEngine]" )
        {
            ecs::ECSEngine engine{};
            auto entity1 = engine.CreateEntity();
            auto entity2 = engine.CreateEntity();

            engine.RegisterComponent< TestComponent >();
            engine.AddComponent< TestComponent >( entity1, 1, 2 );
            engine.AddComponent< TestComponent >( entity2, 3, 4 );

            ecs::EntityHandle patchedEntity = ecs::EntityHandle::NullHandle();
            engine.OnPatch< TestComponent >().emplace_back(
                [&patchedEntity]( ecs::ECSEngine &, ecs::EntityHandle entityHandle )
                { patchedEntity = entityHandle; } );

            engine.PatchComponent< TestComponent >( entity1 );

            REQUIRE( patchedEntity == entity1 );
            REQUIRE( patchedEntity != entity2 );
        }
    }
}