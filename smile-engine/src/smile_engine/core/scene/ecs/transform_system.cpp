/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "transform_system.h"

#include "transform_component.h"
#include "smile_engine/core/ecs/relationship.h"
#include "smile_engine/core/math/math_utilities.h"
#include "smile_engine/core/scene/scene.h"
#include "smile_engine/physics/physics_engine.h"

namespace smile::scene::ecs
{
    TransformSystem::TransformSystem( smile::ecs::ECSEngine *pECSEngine, Scene *pScene )
        : System{ pECSEngine }, m_pScene{ pScene }
    {
        m_pECSEngine->RegisterComponentIfNeeded< smile::ecs::Relationship >();
        m_pECSEngine->RegisterComponentIfNeeded< TransformComponent >();
    }

    void TransformSystem::OnUpdate( primitive::Timestep deltaTime )
    {
        // TODO: Implement dirty flag component and only update the transforms that are dirty
        // For now sort relationship component so that parents and children are grouped

        m_pECSEngine->SortComponent< smile::ecs::Relationship >(
            [pECSEngine = m_pECSEngine](
                const smile::ecs::EntityHandleType lhs, const smile::ecs::EntityHandleType rhs )
            {
                const auto &lhsComp = pECSEngine->GetComponent< smile::ecs::Relationship >( lhs );
                const auto &rhsComp = pECSEngine->GetComponent< smile::ecs::Relationship >( rhs );
                return rhsComp.Parent == lhs || lhsComp.Next == rhs ||
                       ( !( lhsComp.Parent == rhs || rhsComp.Next == lhs ) &&
                           ( lhsComp.Parent < rhsComp.Parent ||
                               ( lhsComp.Parent == rhsComp.Parent && &lhsComp < &rhsComp ) ) );
            } );

        auto view = m_pECSEngine->GetView< TransformComponent >();
        for ( auto entityHandle : view )
        {
            auto &transform = m_pECSEngine->GetComponent< TransformComponent >( entityHandle );

            Entity entity{ entityHandle, m_pScene };

            SM_ASSERT( !( physics::PhysicsEngine::IsPhysicsActor( entity ) &&
                           physics::PhysicsEngine::IsCharacterController( entity ) ),
                "TransformSystem::OnUpdate >> Entity cannot be a physics actor and character controller" );

            if ( physics::PhysicsEngine::IsPhysicsActor( entity ) )
            {
                Ref< physics::PhysicsActor > pActor = physics::PhysicsEngine::GetActorOfEntity( entity );

                if ( transform.TransformChanged &
                     static_cast< Uint32 >( TransformComponent::TransformChanged::Translation ) )
                {
                    pActor->Translate( transform.Translation );
                }

                if ( transform.TransformChanged &
                     static_cast< Uint32 >( TransformComponent::TransformChanged::Rotation ) )
                {
                    pActor->Rotate( transform.Rotation );
                }
            }
            else if ( physics::PhysicsEngine::IsCharacterController( entity ) )
            {
                Ref< physics::CharacterController > pController =
                    physics::PhysicsEngine::GetCharacterControllerOfEntity( entity );

                if ( transform.TransformChanged &
                     static_cast< Uint32 >( TransformComponent::TransformChanged::Translation ) )
                {
                    pController->Translate( transform.Translation );
                }
            }

            transform.TransformChanged = static_cast< Uint32 >( TransformComponent::TransformChanged::None );

            DirectX::XMMATRIX worldTransformMat = DirectX::XMLoadFloat4x4( &transform.GetTransform() );

            auto pRelationship = m_pECSEngine->TryGetComponent< smile::ecs::Relationship >( entityHandle );
            if ( pRelationship && pRelationship->Parent )
            {
                auto parentTransformComp = m_pECSEngine->GetComponent< TransformComponent >( pRelationship->Parent );
                DirectX::XMMATRIX parentTransformMat =
                    DirectX::XMLoadFloat4x4( &parentTransformComp.GetWorldTransform() );
                worldTransformMat *= parentTransformMat;
            }

            DirectX::XMVECTOR worldTranslateVec;
            DirectX::XMVECTOR worldRotationQuatVec;
            DirectX::XMVECTOR worldScaleVec;

            if ( DirectX::XMMatrixDecompose(
                     &worldScaleVec, &worldRotationQuatVec, &worldTranslateVec, worldTransformMat ) )
            {
                DirectX::XMStoreFloat3( &transform.WorldTranslation, worldTranslateVec );

                DirectX::XMFLOAT4 worldRotationQuat;
                DirectX::XMStoreFloat4( &worldRotationQuat, worldRotationQuatVec );
                transform.WorldRotation = math::QuaternionToEuler( worldRotationQuat );

                DirectX::XMStoreFloat3( &transform.WorldScale, worldScaleVec );
            }
        }
    }
}