#include "smpch.h"
#include "transform_system.h"

#include "components.h"
#include "smile_engine/ecs/relationship.h"
#include "smile_engine/math/math_utilities.h"

namespace smile::scene
{
    TransformSystem::TransformSystem( ecs::ECSEngine *pECSEngine ) : System{ pECSEngine }
    {
        m_pECSEngine->RegisterComponentIfNeeded< ecs::Relationship >();
        m_pECSEngine->RegisterComponentIfNeeded< TransformComponent >();
    }

    void TransformSystem::OnUpdate( Timestep deltaTime )
    {
        // TODO: Implement dirty flag component and only update the transforms that are dirty
        // For now sort relationship component so that parents and children are grouped

        m_pECSEngine->SortComponent< ecs::Relationship >(
            [pECSEngine = m_pECSEngine]( const ecs::EntityHandleType lhs, const ecs::EntityHandleType rhs )
            {
                const auto &lhsComp = pECSEngine->GetComponent< ecs::Relationship >( lhs );
                const auto &rhsComp = pECSEngine->GetComponent< ecs::Relationship >( rhs );
                return rhsComp.Parent == lhs || lhsComp.Next == rhs ||
                       ( !( lhsComp.Parent == rhs || rhsComp.Next == lhs ) &&
                           ( lhsComp.Parent < rhsComp.Parent ||
                               ( lhsComp.Parent == rhsComp.Parent && &lhsComp < &rhsComp ) ) );
            } );

        auto view = m_pECSEngine->GetView< TransformComponent >();
        for ( auto entity : view )
        {
            auto &transform = m_pECSEngine->GetComponent< TransformComponent >( entity );

            DirectX::XMMATRIX worldTransformMat = DirectX::XMLoadFloat4x4( &transform.GetTransform() );

            auto pRelationship = m_pECSEngine->TryGetComponent< ecs::Relationship >( entity );
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