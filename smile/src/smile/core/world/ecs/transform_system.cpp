/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "transform_system.h"

#include "transform_component.h"
#include "smile/core/ecs/ecs_engine.h"
#include "smile/core/ecs/relationship.h"
#include "smile/core/math/math_utilities.h"
#include "smile/core/world/world.h"
#include "smile/physics/physics_engine.h"

namespace smile::world::ecs
{
    void TransformSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        ecsEngine.RegisterComponentIfNeeded< smile::ecs::Relationship >();
        ecsEngine.RegisterComponentIfNeeded< TransformComponent >();
        System::OnAdd( ecsEngine );
    }

    void TransformSystem::OnUpdate()
    {
        // TODO: Implement dirty flag component and only update the transforms that are dirty
        // For now sort relationship component so that parents and children are grouped

        m_pECSEngine->SortComponent< smile::ecs::Relationship >(
            [pECSEngine = m_pECSEngine]( const smile::ecs::EntityHandle lhs, const smile::ecs::EntityHandle rhs )
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