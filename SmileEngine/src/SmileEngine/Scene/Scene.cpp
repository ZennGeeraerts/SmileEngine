#include "smpch.h"
#include "Scene.h"

#include "Components.h"
#include "SmileEngine/Renderer/Renderer.h"
#include "SmileEngine/Core/Application.h"
#include "SmileEngine/Physics/PhysicsEngine.h"

#include "Entity.h"

namespace smile
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        m_Registry.clear();
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
        Entity entity{ m_Registry.create(), this };
        entity.AddComponent< IDComponent >( uuid );
        entity.AddComponent< TagComponent >( name );
        entity.AddComponent< TransformComponent >();

        return entity;
    }

    void Scene::DestroyEntity( Entity entity )
    {
        m_Registry.destroy( entity );
    }

    void Scene::OnRuntimeStart()
    {
        PhysicsEngine::CreateScene();

        // Create physisc actors
        {
            const auto &view = m_Registry.view< RigidbodyComponent >();
            for ( auto &e : view )
            {
                Entity entity = { e, this };
                PhysicsEngine::CreateActor( entity );
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        PhysicsEngine::DestroyScene();
    }

    void Scene::OnUpdateRuntime( Timestep deltaTime )
    {
        PhysicsEngine::Simulate( deltaTime );

        Camera *pMainCamera = nullptr;
        DirectX::XMFLOAT4X4 cameraTransform;
        {
            auto view = m_Registry.view< TransformComponent, CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] = view.get< TransformComponent, CameraComponent >( entity );

                if ( camera.m_bPrimary )
                {
                    pMainCamera = &camera.m_Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if ( pMainCamera )
        {
            Renderer::BeginScene( *pMainCamera, cameraTransform );

            {
                auto group = m_Registry.group< MeshRendererComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = group.get< MeshRendererComponent, TransformComponent >( entity );
                    Renderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group = m_Registry.group< StaticMeshComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = group.get< StaticMeshComponent, TransformComponent >( entity );
                    Renderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group = m_Registry.group< SkinnedMeshComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = group.get< SkinnedMeshComponent, TransformComponent >( entity );

                    for ( auto &animator : mesh.m_Animators )
                    {
                        animator.OnUpdate( deltaTime );
                        const auto &boneTransforms = animator.GetBoneTransforms();
                        for ( const auto &pMaterial : mesh.m_pMaterials )
                        {
                            if ( animator.IsPlaying() )
                                pMaterial->GetShader()->UploadMat4Array( "Bones", boneTransforms );
                        }
                    }

                    Renderer::Submit( mesh, transform.GetTransform() );
                }
            }
            {
                auto group = m_Registry.group< BoxColliderComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[boxCollider, transform] =
                        group.get< BoxColliderComponent, TransformComponent >( entity );
                    Renderer::SubmitWireframe( boxCollider, transform.GetTransform() );
                }
            }

            Renderer::OnRender();

            Renderer::EndScene();
        }
    }

    void Scene::OnUpdateEditor( Timestep deltaTime, EditorCamera &editorCamera, bool bSimmulate )
    {
        if ( bSimmulate )
            PhysicsEngine::Simulate( deltaTime );

        Renderer::BeginScene( editorCamera );

        {
            auto group = m_Registry.group< MeshRendererComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< MeshRendererComponent, TransformComponent >( entity );
                Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_Registry.group< StaticMeshComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< StaticMeshComponent, TransformComponent >( entity );
                Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_Registry.group< SkinnedMeshComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< SkinnedMeshComponent, TransformComponent >( entity );

                for ( auto &animator : mesh.m_Animators )
                {
                    animator.OnUpdate( deltaTime );
                    const auto &boneTransforms = animator.GetBoneTransforms();
                    for ( const auto &pMaterial : mesh.m_pMaterials )
                    {
                        if ( animator.IsPlaying() )
                            pMaterial->GetShader()->UploadMat4Array( "Bones", boneTransforms );
                    }
                }

                Renderer::Submit( mesh, transform.GetTransform() );
            }
        }
        {
            auto group = m_Registry.group< BoxColliderComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[boxCollider, transform] = group.get< BoxColliderComponent, TransformComponent >( entity );
                Renderer::SubmitWireframe( boxCollider, transform.GetTransform() );
            }
        }

        Renderer::OnRender();

        Renderer::EndScene();
    }

    void Scene::OnViewportResize( uint32_t width, uint32_t height )
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = view.get< CameraComponent >( entity );
            if ( !cameraComponent.m_bFixedAspectRatio )
            {
                cameraComponent.m_Camera.SetViewportSize( width, height );
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = view.get< CameraComponent >( entity );
            if ( cameraComponent.m_bPrimary )
                return Entity{ entity, this };
        }
        return Entity{};
    }

    template < typename ComponentType >
    static void
    CopyComponent( entt::registry &dst, entt::registry &src, const std::unordered_map< UUID, entt::entity > &enttMap )
    {
        auto view = src.view< ComponentType >();
        for ( auto entity : view )
        {
            UUID uuid = src.get< IDComponent >( entity ).m_ID;
            SM_ASSERT( enttMap.find( uuid ) != enttMap.end(), "Scene > CopyComponent > uuid not found int enttMap" );
            entt::entity dstEnttID = enttMap.at( uuid );

            auto &component = src.get< ComponentType >( entity );
            dst.emplace_or_replace< ComponentType >( dstEnttID, component );
        }
    }

    template < typename ComponentType >
    static void CopyComponentIfExists( Entity dst, Entity src )
    {
        if ( src.HasComponent< ComponentType >() )
            dst.AddOrReplaceComponent< ComponentType >( src.GetComponent< ComponentType >() );
    }

    Ref< Scene > Scene::Copy( const Ref< Scene > &pScene )
    {
        Ref< Scene > pNewScene = CreateRef< Scene >();

        pNewScene->m_ViewportWidth = pScene->m_ViewportWidth;
        pNewScene->m_ViewportHeight = pScene->m_ViewportHeight;

        std::unordered_map< UUID, entt::entity > enttMap{};

        auto &srcSceneRegistry = pScene->m_Registry;
        auto &dstSceneRegistry = pNewScene->m_Registry;
        auto idView = srcSceneRegistry.view< IDComponent >();
        for ( auto entity : idView )
        {
            auto uuid = srcSceneRegistry.get< IDComponent >( entity ).m_ID;
            const auto &name = srcSceneRegistry.get< TagComponent >( entity ).m_Tag;
            Entity newEntity = pNewScene->CreateEntity( uuid, name );
            enttMap[uuid] = static_cast< entt::entity >( newEntity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< TransformComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< MeshRendererComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< StaticMeshComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< SkinnedMeshComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< CameraComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< RigidbodyComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< BoxColliderComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< SphereColliderComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );
        CopyComponent< CapsuleColliderComponent >( dstSceneRegistry, srcSceneRegistry, enttMap );

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
        component.m_Camera.SetViewportSize( m_ViewportWidth, m_ViewportHeight );
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
