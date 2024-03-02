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
        auto pRelationShip = m_pScene->m_ECSEngine.TryGetComponent< ecs::Relationship >( m_EntityHandle );
        if ( !pRelationShip )
        {
            // Create a new relationship component and assign it to the relationship pointer
            // Maybe get the pointer from ECSEngine in the future?
            auto ppRelationship = &pRelationShip;
            auto &comp = m_pScene->m_ECSEngine.AddComponent< ecs::Relationship >( m_EntityHandle );
            *ppRelationship = &comp;
        }

        auto pChildRel = m_pScene->m_ECSEngine.TryGetComponent< ecs::Relationship >( child );
        if ( !pChildRel )
        {
            // Create a new relationship component and assign it to the child relationship pointer
            auto ppChildRel = &pChildRel;
            auto &comp = m_pScene->m_ECSEngine.AddComponent< ecs::Relationship >( child );
            *ppChildRel = &comp;
        }

        if ( pRelationShip->ChildrenCount == 0 )
        {
            pRelationShip->First = child.m_EntityHandle;
        }

        ecs::EntityHandleType end = pRelationShip->First;
        ecs::EntityHandleType next = m_pScene->m_ECSEngine.GetComponent< ecs::Relationship >( end ).Next;
        while ( next != ecs::EntityHandleType::NullHandle() )
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
}