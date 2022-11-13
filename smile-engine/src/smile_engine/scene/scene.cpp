#include "smpch.h"
#include "scene.h"

#include "components.h"
#include "smile_engine/graphic/renderer.h"
#include "smile_engine/core/application.h"
#include "smile_engine/physics/physics_engine.h"

#include "entity.h"

namespace smile::scene
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        m_ECSEngine.Clear();
    }

    Entity Scene::CreateEntity()
    {
        return CreateEntity( "Entity" );
    }

    Entity Scene::CreateEntity( const std::string &name )
    {
        return CreateEntity( UUID{}, name );
    }

    Entity Scene::CreateEntity( UUID uuid, const std::string &name )
    {
        Entity entity{ m_ECSEngine.CreateEntity(), this };
        entity.AddComponent< IDComponent >( uuid );
        entity.AddComponent< TagComponent >( name );
        entity.AddComponent< TransformComponent >();

        return entity;
    }

    void Scene::DestroyEntity( Entity entity )
    {
        m_ECSEngine.DestroyEntity( entity );
    }

    void Scene::OnRuntimeStart()
    {
        physics::PhysicsEngine::CreateScene();

        // Create physisc actors
        {
            auto view = m_ECSEngine.GetView< RigidbodyComponent >();
            for ( auto e : view )
            {
                Entity entity = { e, this };
                physics::PhysicsEngine::CreateActor( entity );
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        physics::PhysicsEngine::DestroyScene();
    }

    void Scene::OnSimulationStart()
    {
        physics::PhysicsEngine::CreateScene();

        // Create physisc actors
        {
            auto view = m_ECSEngine.GetView< RigidbodyComponent >();
            for ( auto e : view )
            {
                Entity entity = { e, this };
                physics::PhysicsEngine::CreateActor( entity );
            }
        }
    }

    void Scene::OnSimulationStop()
    {
        physics::PhysicsEngine::DestroyScene();
    }

    void Scene::OnUpdateRuntime( Timestep deltaTime )
    {
        physics::PhysicsEngine::Simulate( deltaTime );

        graphic::Camera *pMainCamera = nullptr;
        DirectX::XMFLOAT4X4 cameraTransform;
        {
            auto view = m_ECSEngine.GetView< TransformComponent, CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] = m_ECSEngine.GetComponents< TransformComponent, CameraComponent >( entity );

                if ( camera.IsPrimary )
                {
                    pMainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if ( pMainCamera )
        {
            graphic::Renderer::BeginScene( *pMainCamera, cameraTransform );

            {
                auto group = m_ECSEngine.GetGroup< MeshRendererComponent >( ecs::g_Get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = m_ECSEngine.GetComponents< MeshRendererComponent, TransformComponent >( entity );
                    graphic::Renderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group = m_ECSEngine.GetGroup< StaticMeshComponent >( ecs::g_Get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = m_ECSEngine.GetComponents< StaticMeshComponent, TransformComponent >( entity );
                    graphic::Renderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group = m_ECSEngine.GetGroup< SkinnedMeshComponent >( ecs::g_Get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = m_ECSEngine.GetComponents< SkinnedMeshComponent, TransformComponent >( entity );

                    for ( auto &animator : mesh.Animators )
                    {
                        animator.OnUpdate( deltaTime );
                        const auto &boneTransforms = animator.GetBoneTransforms();
                        for ( const auto &pMaterial : mesh.pMaterials )
                        {
                            if ( animator.IsPlaying() )
                                pMaterial->GetShader()->UploadMat4Array( "Bones", boneTransforms );
                        }
                    }

                    graphic::Renderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group = m_ECSEngine.GetGroup< BoxColliderComponent >( ecs::g_Get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[boxCollider, transform] =
                        m_ECSEngine.GetComponents< BoxColliderComponent, TransformComponent >( entity );
                    graphic::Renderer::SubmitWireframe( boxCollider, transform.GetTransform() );
                }
            }

            graphic::Renderer::OnRender();

            graphic::Renderer::EndScene();
        }
    }

    void Scene::OnUpdateSimulation( Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        physics::PhysicsEngine::Simulate( deltaTime );

        graphic::Renderer::BeginScene( editorCamera );

        {
            auto group = m_ECSEngine.GetGroup< MeshRendererComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = m_ECSEngine.GetComponents< MeshRendererComponent, TransformComponent >( entity );
                graphic::Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< StaticMeshComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = m_ECSEngine.GetComponents< StaticMeshComponent, TransformComponent >( entity );
                graphic::Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< SkinnedMeshComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] =
                    m_ECSEngine.GetComponents< SkinnedMeshComponent, TransformComponent >( entity );

                for ( auto &animator : mesh.Animators )
                {
                    animator.OnUpdate( deltaTime );
                    const auto &boneTransforms = animator.GetBoneTransforms();
                    for ( const auto &pMaterial : mesh.pMaterials )
                    {
                        if ( animator.IsPlaying() )
                            pMaterial->GetShader()->UploadMat4Array( "Bones", boneTransforms );
                    }
                }

                graphic::Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< BoxColliderComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[boxCollider, transform] =
                    m_ECSEngine.GetComponents< BoxColliderComponent, TransformComponent >( entity );
                graphic::Renderer::SubmitWireframe( boxCollider, transform.GetTransform() );
            }
        }

        graphic::Renderer::OnRender();

        graphic::Renderer::EndScene();
    }

    void Scene::OnUpdateEditor( Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        graphic::Renderer::BeginScene( editorCamera );

        {
            auto group = m_ECSEngine.GetGroup< MeshRendererComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = m_ECSEngine.GetComponents< MeshRendererComponent, TransformComponent >( entity );
                graphic::Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< StaticMeshComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = m_ECSEngine.GetComponents< StaticMeshComponent, TransformComponent >( entity );
                graphic::Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< SkinnedMeshComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = m_ECSEngine.GetComponents< SkinnedMeshComponent, TransformComponent >( entity );

                for ( auto &animator : mesh.Animators )
                {
                    animator.OnUpdate( deltaTime );
                    const auto &boneTransforms = animator.GetBoneTransforms();
                    for ( const auto &pMaterial : mesh.pMaterials )
                    {
                        if ( animator.IsPlaying() )
                            pMaterial->GetShader()->UploadMat4Array( "Bones", boneTransforms );
                    }
                }

                graphic::Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_ECSEngine.GetGroup< BoxColliderComponent >( ecs::g_Get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[boxCollider, transform] =
                    m_ECSEngine.GetComponents< BoxColliderComponent, TransformComponent >( entity );
                graphic::Renderer::SubmitWireframe( boxCollider, transform.GetTransform() );
            }
        }

        graphic::Renderer::OnRender();

        graphic::Renderer::EndScene();
    }

    void Scene::OnViewportResize( Uint32 width, Uint32 height )
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_ECSEngine.GetView< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = m_ECSEngine.GetComponent< CameraComponent >( entity );
            if ( !cameraComponent.HasFixedAspectRatio )
            {
                cameraComponent.Camera.SetViewportSize( width, height );
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_ECSEngine.GetView< CameraComponent >();
        for ( auto entity : view )
        {
            auto &camera_component = m_ECSEngine.GetComponent< CameraComponent >( entity );
            if ( camera_component.IsPrimary )
                return Entity{ entity, this };
        }
        return Entity{};
    }

    template < typename ComponentType >
    static void CopyComponent( ecs::ECSEngine &dst,
        ecs::ECSEngine &src,
        const std::unordered_map< UUID, ecs::EntityHandleType > &entityHandleMap )
    {
        auto view = src.GetView< ComponentType >();
        for ( auto entity : view )
        {
            UUID uuid = src.GetComponent< IDComponent >( entity ).ID;
            SM_ASSERT( entityHandleMap.find( uuid ) != entityHandleMap.end(), "Scene::CopyComponent > uuid not found int enttMap" );
            ecs::EntityHandleType dstHandleID = entityHandleMap.at( uuid );

            auto &component = src.GetComponent< ComponentType >( entity );
            dst.AddOrReplaceComponent< ComponentType >( dstHandleID, component );
        }
    }

    template < typename ComponentType >
    static void CopyComponentIfExists( Entity dst, Entity src )
    {
        if ( src.HasComponent< ComponentType >() )
            dst.AddOrReplaceComponent< ComponentType >( src.GetComponent< ComponentType >() );
    }

    Ref< Scene > Scene::Copy( const Ref< Scene > &scene )
    {
        Ref< Scene > pNewScene = CreateRef< Scene >();

        pNewScene->m_ViewportWidth = scene->m_ViewportWidth;
        pNewScene->m_ViewportHeight = scene->m_ViewportHeight;

        std::unordered_map< UUID, ecs::EntityHandleType > entityMap{};

        auto &srcSceneEngine = scene->m_ECSEngine;
        auto &dstSceneEngine = pNewScene->m_ECSEngine;
        auto idView = srcSceneEngine.GetView< IDComponent >();
        for ( auto entity : idView )
        {
            auto uuid = srcSceneEngine.GetComponent< IDComponent >( entity ).ID;
            const auto &name = srcSceneEngine.GetComponent< TagComponent >( entity ).Tag;
            Entity newEntity = pNewScene->CreateEntity( uuid, name );
            entityMap[uuid] = static_cast< ecs::EntityHandleType >( newEntity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< TransformComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< MeshRendererComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< StaticMeshComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< SkinnedMeshComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< CameraComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< RigidbodyComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< BoxColliderComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< SphereColliderComponent >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent< CapsuleColliderComponent >( dstSceneEngine, srcSceneEngine, entityMap );

        return pNewScene;
    }

    void Scene::DuplicateEntity( Entity entity )
    {
        Entity newEntity = CreateEntity( entity.GetName() );

        CopyComponentIfExists< TransformComponent >( newEntity, entity );
        CopyComponentIfExists< MeshRendererComponent >( newEntity, entity );
        CopyComponentIfExists< StaticMeshComponent >( newEntity, entity );
        CopyComponentIfExists< SkinnedMeshComponent >( newEntity, entity );
        CopyComponentIfExists< CameraComponent >( newEntity, entity );
        CopyComponentIfExists< RigidbodyComponent >( newEntity, entity );
        CopyComponentIfExists< BoxColliderComponent >( newEntity, entity );
        CopyComponentIfExists< SphereColliderComponent >( newEntity, entity );
        CopyComponentIfExists< CapsuleColliderComponent >( newEntity, entity );
    }

    template < typename ComponentType >
    void Scene::OnComponentAdded( Entity entity, ComponentType &component )
    {
        static_assert( false );
    }

    template <>
    void Scene::OnComponentAdded< IDComponent >( Entity entity, IDComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< TagComponent >( Entity entity, TagComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< TransformComponent >( Entity entity, TransformComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< CameraComponent >( Entity entity, CameraComponent &component )
    {
        component.Camera.SetViewportSize( m_ViewportWidth, m_ViewportHeight );
    }

    template <>
    void Scene::OnComponentAdded< MeshRendererComponent >( Entity entity, MeshRendererComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< StaticMeshComponent >( Entity entity, StaticMeshComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< SkinnedMeshComponent >( Entity entity, SkinnedMeshComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< RigidbodyComponent >( Entity entity, RigidbodyComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< BoxColliderComponent >( Entity entity, BoxColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< SphereColliderComponent >( Entity entity, SphereColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< CapsuleColliderComponent >( Entity entity, CapsuleColliderComponent &component )
    {
    }
}
