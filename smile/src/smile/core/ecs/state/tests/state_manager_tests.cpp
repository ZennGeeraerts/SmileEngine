/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/core/ecs/state/state_manager.h"
#include "smile/core/ecs/state/system_factory.h"
#include "smile/core/ecs/ecs_engine.h"
#include <catch/catch.hpp>

namespace smile
{
    namespace ecsstatemanagertests
    {
        class TestComponent;
        class FooComponent;
        class BarComponent;

        class TestSystem : public ecs::System< TestSystem >
        {
          public:
            void OnUpdate() override
            {
            }
        };

        class TestFooSystem : public ecs::System< TestFooSystem >
        {
          public:
            void OnUpdate() override
            {
            }
        };

        class TestBarSystem : public ecs::System< TestBarSystem >
        {
          public:
            void OnUpdate() override
            {
            }
        };
    }

    TEST_CASE( "ecs state manager", "[ecs]" )
    {
        SECTION( "main" )
        {
            using namespace ecsstatemanagertests;
            ecs::state::SystemFactory::RegisterSystem< TestSystem >();
            ecs::state::SystemFactory::RegisterSystem< TestFooSystem >();
            ecs::state::SystemFactory::RegisterSystem< TestBarSystem >();

            ecs::ECSEngine engine;

            auto pTestSystem = memory::CreateRef< TestSystem >();
            engine.AddSystem( pTestSystem );

            auto pDefaultState = memory::CreateRef< ecs::state::State >();
            auto pFooState = memory::CreateRef< ecs::state::State >();
            auto pBarState = memory::CreateRef< ecs::state::State >();
            auto pFullState = memory::CreateRef< ecs::state::State >();

            pFooState->AddSystem( std::string{ TestFooSystem::GetStaticName() } );
            pBarState->AddSystem( std::string{ TestBarSystem::GetStaticName() } );

            pFullState->AddSystem( std::string{ TestFooSystem::GetStaticName() } );
            pFullState->AddSystem( std::string{ TestBarSystem::GetStaticName() } );

            ecs::state::StateManager stateManager;

            stateManager.AddState( "default", pDefaultState );
            stateManager.AddState( "foo", pFooState );
            stateManager.AddState( "bar", pBarState );
            stateManager.AddState( "full", pFullState );

            stateManager.Initialize( &engine, "default" );

            REQUIRE( engine.GetSystems().size() == 1 );
            CHECK( engine.GetSystems()[0] == pTestSystem );

            stateManager.ChangeState( "foo" );

            REQUIRE( engine.GetSystems().size() == 2 );
            CHECK( engine.GetSystems()[0] == pTestSystem );
            CHECK( engine.GetSystems()[1]->GetName() == "smile::ecsstatemanagertests::TestFooSystem" );

            stateManager.ChangeState( "bar" );

            REQUIRE( engine.GetSystems().size() == 2 );
            CHECK( engine.GetSystems()[0] == pTestSystem );
            CHECK( engine.GetSystems()[1]->GetName() == "smile::ecsstatemanagertests::TestBarSystem" );

            stateManager.ChangeState( "full" );

            REQUIRE( engine.GetSystems().size() == 3 );
            CHECK( engine.GetSystems()[0] == pTestSystem );
            CHECK( engine.GetSystems()[1]->GetName() == "smile::ecsstatemanagertests::TestBarSystem" );
            CHECK( engine.GetSystems()[2]->GetName() == "smile::ecsstatemanagertests::TestFooSystem" );
        }
    }
}