#include "smpch.h"
#include "entity.h"

#include "engine/core/ecs/relationship.h"

namespace smile::world
{
    Entity::Entity( smile::ecs::EntityHandle handle, World *pWorld ) : m_EntityHandle{ handle }, m_pWorld{ pWorld }
    {
    }

    void Entity::AddChild( Entity child )
    {
        auto pRelationShip = TryGetComponent< smile::ecs::Relationship >();
        if ( !pRelationShip )
        {
            // Create a new relationship component and assign it to the relationship pointer
            // Maybe get the pointer from ECSEngine in the future?
            auto ppRelationship = &pRelationShip;
            auto &comp = AddComponent< smile::ecs::Relationship >();
            *ppRelationship = &comp;
        }

        auto pChildRel = child.TryGetComponent< smile::ecs::Relationship >();
        if ( !pChildRel )
        {
            // Create a new relationship component and assign it to the child relationship pointer
            auto ppChildRel = &pChildRel;
            auto &comp = child.AddComponent< smile::ecs::Relationship >();
            *ppChildRel = &comp;
        }

        if ( pRelationShip->ChildrenCount == 0 )
        {
            pRelationShip->First = child;
        }

        smile::ecs::EntityHandle end = pRelationShip->First;
        smile::ecs::EntityHandle next = m_pWorld->m_ECSEngine.GetComponent< smile::ecs::Relationship >( end ).Next;
        while ( next )
        {
            end = next;
            next = m_pWorld->m_ECSEngine.GetComponent< smile::ecs::Relationship >( end ).Next;
        }

        if ( pRelationShip->ChildrenCount != 0 )
        {
            auto &endRel = m_pWorld->m_ECSEngine.GetComponent< smile::ecs::Relationship >( end );
            endRel.Next = child;
            pChildRel->Prev = end;
        }

        pChildRel->Parent = m_EntityHandle;

        ++pRelationShip->ChildrenCount;
    }

    void Entity::RemoveChild( Entity child )
    {
        SM_ASSERT( HasComponent< smile::ecs::Relationship >() && child.HasComponent< smile::ecs::Relationship >(),
            "Entity::RemoveChild > Child does not exist" );

        auto &relationship = GetComponent< smile::ecs::Relationship >();
        auto &childRel = child.GetComponent< smile::ecs::Relationship >();

        SM_ASSERT( relationship.ChildrenCount > 0, "Entity::RemoveChild > Entity does not have children" );
        SM_ASSERT( childRel.Parent == m_EntityHandle, "Entity::RemoveChild > Not a child of this entity" );

        if ( relationship.First == child )
            relationship.First = childRel.Next;

        if ( childRel.Next )
            m_pWorld->m_ECSEngine.GetComponent< smile::ecs::Relationship >( childRel.Next ).Prev = childRel.Prev;

        if ( childRel.Prev )
            m_pWorld->m_ECSEngine.GetComponent< smile::ecs::Relationship >( childRel.Prev ).Next = childRel.Next;

        childRel.Next = smile::ecs::EntityHandle::NullHandle();
        childRel.Prev = smile::ecs::EntityHandle::NullHandle();
        childRel.Parent = smile::ecs::EntityHandle::NullHandle();

        --relationship.ChildrenCount;
    }

    Uint32 Entity::GetChildrenCount() const
    {
        auto pRelationShip = TryGetComponent< smile::ecs::Relationship >();
        return pRelationShip ? pRelationShip->ChildrenCount : 0;
    }

    Entity Entity::GetChildAtIndex( const Uint32 index ) const
    {
        SM_ASSERT(
            HasComponent< smile::ecs::Relationship >(), "Entity::GetChildAtIndex > Entity does not have children" );

        auto &relationship = GetComponent< smile::ecs::Relationship >();

        SM_ASSERT( relationship.ChildrenCount > index, "Entity::GetChildAtIndex > Index out of range" );

        auto currentChildHandle = relationship.First;
        for ( Uint32 i{}; i < relationship.ChildrenCount; ++i )
        {
            if ( i == index )
                return Entity{ currentChildHandle, m_pWorld };

            currentChildHandle =
                m_pWorld->m_ECSEngine.GetComponent< smile::ecs::Relationship >( currentChildHandle ).Next;
        }

        SM_ASSERT( false, "Entity::GetChildAtIndex > Failed to get child at index: %d", index );
        return Entity{};
    }

    Entity Entity::GetParent() const
    {
        auto pRelationShip = TryGetComponent< smile::ecs::Relationship >();
        if ( !pRelationShip || !pRelationShip->Parent )
            return Entity{};

        return Entity{ pRelationShip->Parent, m_pWorld };
    }
}