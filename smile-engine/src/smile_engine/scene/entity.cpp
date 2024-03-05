#include "smpch.h"
#include "entity.h"

#include "smile_engine/ecs/relationship.h"

namespace smile::scene
{
    Entity::Entity( ecs::EntityHandleType handle, Scene *pScene ) : m_EntityHandle{ handle }, m_pScene{ pScene }
    {
    }

    void Entity::AddChild( Entity child )
    {
        auto pRelationShip = TryGetComponent< ecs::Relationship >();
        if ( !pRelationShip )
        {
            // Create a new relationship component and assign it to the relationship pointer
            // Maybe get the pointer from ECSEngine in the future?
            auto ppRelationship = &pRelationShip;
            auto &comp = AddComponent< ecs::Relationship >();
            *ppRelationship = &comp;
        }

        auto pChildRel = child.TryGetComponent< ecs::Relationship >();
        if ( !pChildRel )
        {
            // Create a new relationship component and assign it to the child relationship pointer
            auto ppChildRel = &pChildRel;
            auto &comp = child.AddComponent< ecs::Relationship >();
            *ppChildRel = &comp;
        }

        if ( pRelationShip->ChildrenCount == 0 )
        {
            pRelationShip->First = child;
        }

        ecs::EntityHandleType end = pRelationShip->First;
        ecs::EntityHandleType next = m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( end ).Next;
        while ( next )
        {
            end = next;
            next = m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( end ).Next;
        }

        if ( pRelationShip->ChildrenCount != 0 )
        {
            auto &endRel = m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( end );
            endRel.Next = child;
            pChildRel->Prev = end;
        }

        pChildRel->Parent = m_EntityHandle;

        ++pRelationShip->ChildrenCount;
    }

    void Entity::RemoveChild( Entity child )
    {
        SM_ASSERT( HasComponent< ecs::Relationship >() && child.HasComponent< ecs::Relationship >(),
            "Entity::RemoveChild > Child does not exist" );

        auto &relationship = GetComponent< ecs::Relationship >();
        auto &childRel = child.GetComponent< ecs::Relationship >();

        SM_ASSERT( relationship.ChildrenCount > 0, "Entity::RemoveChild > Entity does not have children" );
        SM_ASSERT( childRel.Parent == m_EntityHandle, "Entity::RemoveChild > Not a child of this entity" );

        if ( relationship.First == child )
            relationship.First = childRel.Next;

        if ( childRel.Next )
            m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( childRel.Next ).Prev = childRel.Prev;

        if ( childRel.Prev )
            m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( childRel.Prev ).Next = childRel.Next;

        childRel.Next = ecs::EntityHandleType::NullHandle();
        childRel.Prev = ecs::EntityHandleType::NullHandle();
        childRel.Parent = ecs::EntityHandleType::NullHandle();

        --relationship.ChildrenCount;
    }

    Uint32 Entity::GetChildrenCount() const
    {
        auto pRelationShip = TryGetComponent< ecs::Relationship >();
        return pRelationShip ? pRelationShip->ChildrenCount : 0;
    }

    Entity Entity::GetChildAtIndex( const Uint32 index ) const
    {
        SM_ASSERT( HasComponent< ecs::Relationship >(), "Entity::GetChildAtIndex > Entity does not have children" );

        auto &relationship = GetComponent< ecs::Relationship >();

        SM_ASSERT( relationship.ChildrenCount > index, "Entity::GetChildAtIndex > Index out of range" );

        auto currentChildHandle = relationship.First;
        for ( Uint32 i{}; i < relationship.ChildrenCount; ++i )
        {
            if ( i == index )
                return Entity{ currentChildHandle, m_pScene };

            currentChildHandle = m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( currentChildHandle ).Next;
        }

        SM_ASSERT( false, "Entity::GetChildAtIndex > Failed to get child at index: %d", index );
        return Entity{};
    }

    Entity Entity::GetParent() const
    {
        auto pRelationShip = TryGetComponent< ecs::Relationship >();
        if ( !pRelationShip || !pRelationShip->Parent )
            return Entity{};

        return Entity{ pRelationShip->Parent, m_pScene };
    }
}