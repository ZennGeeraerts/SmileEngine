#include "smile/engine/core/scene/entity.h"
#include "smile/engine/core/ecs/relationship.h"
#include <catch/catch.hpp>

using namespace smile;

TEST_CASE( "Entity" )
{
    SECTION( "AddChild" )
    {
        Ref< scene::Scene > pScene = CreateRef< scene::Scene >();

        scene::Entity parent = pScene->CreateEntity();
        scene::Entity child1 = pScene->CreateEntity();
        scene::Entity child2 = pScene->CreateEntity();
        scene::Entity child3 = pScene->CreateEntity();

        parent.AddChild( child1 );

        const auto &parentRelationship = parent.GetComponent< ecs::Relationship >();

        REQUIRE( parentRelationship.ChildrenCount == 1 );
        REQUIRE( parentRelationship.First == child1 );
        REQUIRE( parentRelationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( parentRelationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( parentRelationship.Parent == ecs::EntityHandleType::NullHandle() );

        const auto &child1Relationship = child1.GetComponent< ecs::Relationship >();

        REQUIRE( child1Relationship.ChildrenCount == 0 );
        REQUIRE( child1Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Parent == parent );

        parent.AddChild( child2 );
        parent.AddChild( child3 );

        REQUIRE( parentRelationship.ChildrenCount == 3 );
        REQUIRE( parentRelationship.First == child1 );
        REQUIRE( parentRelationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( parentRelationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( parentRelationship.Parent == ecs::EntityHandleType::NullHandle() );

        REQUIRE( child1Relationship.ChildrenCount == 0 );
        REQUIRE( child1Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Next == child2 );
        REQUIRE( child1Relationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Parent == parent );

        const auto &child2Relationship = child2.GetComponent< ecs::Relationship >();

        REQUIRE( child2Relationship.ChildrenCount == 0 );
        REQUIRE( child2Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child2Relationship.Next == child3 );
        REQUIRE( child2Relationship.Prev == child1 );
        REQUIRE( child2Relationship.Parent == parent );

        const auto &child3Relationship = child3.GetComponent< ecs::Relationship >();

        REQUIRE( child3Relationship.ChildrenCount == 0 );
        REQUIRE( child3Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child3Relationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child3Relationship.Prev == child2 );
        REQUIRE( child3Relationship.Parent == parent );
    }

    SECTION( "RemoveChild" )
    {
        Ref< scene::Scene > pScene = CreateRef< scene::Scene >();

        scene::Entity parent = pScene->CreateEntity();
        scene::Entity child1 = pScene->CreateEntity();
        scene::Entity child2 = pScene->CreateEntity();
        scene::Entity child3 = pScene->CreateEntity();

        parent.AddChild( child1 );
        parent.AddChild( child2 );
        parent.AddChild( child3 );

        parent.RemoveChild( child2 );

        const auto &parentRelationship = parent.GetComponent< ecs::Relationship >();
        const auto &child1Relationship = child1.GetComponent< ecs::Relationship >();
        const auto &child2Relationship = child2.GetComponent< ecs::Relationship >();
        const auto &child3Relationship = child3.GetComponent< ecs::Relationship >();

        REQUIRE( parentRelationship.ChildrenCount == 2 );
        REQUIRE( parentRelationship.First == child1 );
        REQUIRE( parentRelationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( parentRelationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( parentRelationship.Parent == ecs::EntityHandleType::NullHandle() );

        REQUIRE( child1Relationship.ChildrenCount == 0 );
        REQUIRE( child1Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Next == child3 );
        REQUIRE( child1Relationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child1Relationship.Parent == parent );

        REQUIRE( child2Relationship.ChildrenCount == 0 );
        REQUIRE( child2Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child2Relationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child2Relationship.Prev == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child2Relationship.Parent == ecs::EntityHandleType::NullHandle() );

        REQUIRE( child3Relationship.ChildrenCount == 0 );
        REQUIRE( child3Relationship.First == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child3Relationship.Next == ecs::EntityHandleType::NullHandle() );
        REQUIRE( child3Relationship.Prev == child1 );
        REQUIRE( child3Relationship.Parent == parent );
    }
}