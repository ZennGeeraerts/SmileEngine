/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physics_system.h"

#include "ecs/ecs_engine.h"
#include "world/ecs/id_component.h"
#include "world/ecs/transform_component.h"
#include "math/math.h"

#include "rigidbody_component.h"
#include "character_controller_component.h"
#include "box_collider_component.h"
#include "sphere_collider_component.h"
#include "capsule_collider_component.h"

#include "engine/physics/physics_engine.h"

namespace smile::physics::ecs
{
    void PhysicsSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        System::OnAdd( ecsEngine );

        m_pPhysicsWorld = PhysicsEngine::GetInstance().CreateWorld( PhysicsWorldSettings{} );

        // Create rigidbodies
        {
            auto group = ecsEngine.GetGroup< RigidbodyComponent >(
                smile::ecs::g_Get< world::ecs::IDComponent, world::ecs::TransformComponent > );

            auto onRigidbodyAddedFunc = [&]( smile::ecs::ECSEngine &ecsEngine, smile::ecs::EntityHandleType entity )
            {
                const auto &[rigidbodyComponent, idComponent, transformComponent] =
                    ecsEngine
                        .GetComponents< RigidbodyComponent, world::ecs::IDComponent, world::ecs::TransformComponent >(
                            entity );

                Ref< Rigidbody > pRigidbody =
                    m_pPhysicsWorld->CreateRigidbody( rigidbodyComponent.BodyType, transformComponent.GetTransform() );

                if ( pRigidbody->IsDynamic() )
                {
                    pRigidbody->SetLinearDrag( rigidbodyComponent.LinearDrag );
                    pRigidbody->SetAngularDrag( rigidbodyComponent.AngularDrag );
                    pRigidbody->SetKinematic( rigidbodyComponent.IsKinematic );
                    pRigidbody->SetCollisionDetection( rigidbodyComponent.CollisionDetection );
                    pRigidbody->LockTranslation( rigidbodyComponent.LockPositionX,
                        rigidbodyComponent.LockPositionY,
                        rigidbodyComponent.LockPositionZ );
                    pRigidbody->LockRotation( rigidbodyComponent.LockRotationX,
                        rigidbodyComponent.LockRotationY,
                        rigidbodyComponent.LockRotationZ );
                    pRigidbody->SetMass( rigidbodyComponent.Mass );
                }

                pRigidbody->SetDisableGravity( rigidbodyComponent.DisableGravity );

                if ( auto pBoxColliderComponent = ecsEngine.TryGetComponent< ecs::BoxColliderComponent >( entity ) )
                {
                    geometric::Box box = pBoxColliderComponent->Box;
                    if ( transformComponent.WorldScale.x != 0.0f )
                        box.Size.x = box.Size.x * transformComponent.WorldScale.x / 2.0f;
                    if ( transformComponent.WorldScale.y != 0.0f )
                        box.Size.y = box.Size.y * transformComponent.WorldScale.y / 2.0f;
                    if ( transformComponent.WorldScale.z != 0.0f )
                        box.Size.z = box.Size.z * transformComponent.WorldScale.z / 2.0f;

                    auto boxGeometry = PhysicsBoxGeometry{ box };
                    Ref< PhysicsShape > pPhysicsShape = pRigidbody->CreateShape( &boxGeometry );
                    pPhysicsShape->SetTrigger( pBoxColliderComponent->IsTrigger );
                }

                if ( auto pSphereColliderComponent =
                         ecsEngine.TryGetComponent< ecs::SphereColliderComponent >( entity ) )
                {
                    float radius = pSphereColliderComponent->Sphere.Radius;
                    if ( transformComponent.WorldScale.x != 0.0f )
                        radius *= transformComponent.WorldScale.x;

                    auto sphereGeometry = PhysicsSphereGeometry{ geometric::Sphere{ DirectX::XMFLOAT3{}, radius } };
                    Ref< PhysicsShape > pPhysicsShape = pRigidbody->CreateShape( &sphereGeometry );
                    pPhysicsShape->SetTrigger( pSphereColliderComponent->IsTrigger );
                }

                if ( auto pCapsuleColliderComponent =
                         ecsEngine.TryGetComponent< ecs::CapsuleColliderComponent >( entity ) )
                {
                    const float radius = pCapsuleColliderComponent->Capsule.Radius *
                                         std::max( transformComponent.WorldScale.x, transformComponent.WorldScale.z );

                    auto capsuleGeometry = PhysicsCapsuleGeometry{ geometric::Capsule{ DirectX::XMFLOAT3{},
                        radius,
                        pCapsuleColliderComponent->Capsule.Height / 2.0f * transformComponent.WorldScale.y } };
                    Ref< PhysicsShape > pPhysicsShape = pRigidbody->CreateShape( &capsuleGeometry );
                    pPhysicsShape->SetTrigger( pCapsuleColliderComponent->IsTrigger );
                }

                m_RigidbodyMap[idComponent.ID] = pRigidbody;
            };

            ecsEngine.OnConstruction< RigidbodyComponent >().emplace_back( onRigidbodyAddedFunc );

            for ( auto entityHandle : group )
            {
                onRigidbodyAddedFunc( ecsEngine, entityHandle );
            }
        }
        // Create character controllers
        {
            auto group = ecsEngine.GetGroup< CharacterControllerComponent >(
                smile::ecs::g_Get< world::ecs::IDComponent, world::ecs::TransformComponent > );

            auto onCharacterControllerAddedFunc = [&]( smile::ecs::ECSEngine &ecsEngine, smile::ecs::EntityHandleType entity )
            {
                const auto &[characterControllerComponent, idComponent, transformComponent] =
                    ecsEngine.GetComponents< CharacterControllerComponent,
                        world::ecs::IDComponent,
                        world::ecs::TransformComponent >( entity );

                Ref< CharacterController > pCharacterController =
                    m_pPhysicsWorld->CreateCharacterController( characterControllerComponent.Radius,
                        characterControllerComponent.Height,
                        characterControllerComponent.ClimbingMode,
                        transformComponent.Translation );

                pCharacterController->SetName( characterControllerComponent.Name );
                pCharacterController->SetCollisionGroups(
                    characterControllerComponent.CollisionGroups, characterControllerComponent.CollisionIgnoreGroups );

                m_CharacterControllerMap[idComponent.ID] = pCharacterController;
            };

            ecsEngine.OnConstruction< CharacterControllerComponent >().emplace_back( onCharacterControllerAddedFunc );

            for ( auto entityHandle : group )
            {
                onCharacterControllerAddedFunc( ecsEngine, entityHandle );
            }
        }
    }

    void PhysicsSystem::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        ecsEngine.OnConstruction< RigidbodyComponent >().clear();
        ecsEngine.OnConstruction< CharacterControllerComponent >().clear();

        System::OnRemove( ecsEngine );

        m_RigidbodyMap.clear();
        m_CharacterControllerMap.clear();

        PhysicsEngine::GetInstance().DestroyWorld( m_pPhysicsWorld );
        m_pPhysicsWorld = nullptr;
    }

    void PhysicsSystem::OnUpdate( primitive::Timestep deltaTime )
    {
        if ( PhysicsEngine::GetInstance().OnSimulate( deltaTime ) )
        {
            {
                auto group = m_pECSEngine->GetGroup< RigidbodyComponent >(
                    smile::ecs::g_Get< world::ecs::IDComponent, world::ecs::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[rigidbodyComponent, idComponent, transformComponent] =
                        m_pECSEngine->GetComponents< RigidbodyComponent,
                            world::ecs::IDComponent,
                            world::ecs::TransformComponent >( entity );

                    transformComponent.Translation = m_RigidbodyMap[idComponent.ID]->GetPosition();
                    transformComponent.Rotation =
                        math::QuaternionToEuler( m_RigidbodyMap[idComponent.ID]->GetRotation() );

                    if ( transformComponent.TransformChanged &
                         static_cast< Uint32 >( world::ecs::TransformComponent::TransformChanged::Translation ) )
                    {
                        m_RigidbodyMap[idComponent.ID]->Translate( transformComponent.WorldTranslation );
                    }

                    if ( transformComponent.TransformChanged &
                         static_cast< Uint32 >( world::ecs::TransformComponent::TransformChanged::Rotation ) )
                    {
                        m_RigidbodyMap[idComponent.ID]->Rotate( transformComponent.WorldRotation );
                    }

                    transformComponent.TransformChanged =
                        static_cast< Uint32 >( world::ecs::TransformComponent::TransformChanged::None );
                }
            }
            {
                auto group = m_pECSEngine->GetGroup< CharacterControllerComponent >(
                    smile::ecs::g_Get< world::ecs::IDComponent, world::ecs::TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[characterControllerComponent, idComponent, transformComponent] =
                        m_pECSEngine->GetComponents< CharacterControllerComponent,
                            world::ecs::IDComponent,
                            world::ecs::TransformComponent >( entity );

                    transformComponent.Translation = m_CharacterControllerMap[idComponent.ID]->GetPosition();

                    if ( transformComponent.TransformChanged &
                         static_cast< Uint32 >( world::ecs::TransformComponent::TransformChanged::Translation ) )
                    {
                        m_CharacterControllerMap[idComponent.ID]->Translate( transformComponent.WorldTranslation );
                    }

                    transformComponent.TransformChanged =
                        static_cast< Uint32 >( world::ecs::TransformComponent::TransformChanged::None );
                }
            }
        }
    }
}