#include "smpch.h"
#include "script_glue.h"

#include "script_engine.h"

#include "smile_engine/input/key_codes.h"
#include "smile_engine/input/input.h"

#include "smile_engine/physics/physics_engine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <string_view>

namespace smile::scripting
{
    static std::unordered_map< MonoType *, std::function< bool( scene::Entity ) > > s_EntityHasComponentFuncs{};

#define SM_ADD_INTERNAL_CALL( name ) mono_add_internal_call( "Smile.InternalCalls::" #name, name )

    static bool Entity_HasComponent( UUID entityID, MonoReflectionType *pComponentType )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        SM_ASSERT( pScene, "" );
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        SM_ASSERT( entity, "" );

        MonoType *pManagedType = mono_reflection_type_get_type( pComponentType );
        SM_ASSERT( s_EntityHasComponentFuncs.find( pManagedType ) != s_EntityHasComponentFuncs.end(),
            "ScriptGlue::Entity_HasComponent > UUnregistered type" );

        return s_EntityHasComponentFuncs.at( pManagedType )( entity );
    }

    static void TransformComponent_GetTranslation( UUID entityID, DirectX::XMFLOAT3 *pOutTranslation )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        *pOutTranslation = entity.GetComponent< scene::TransformComponent >().Translation;
    } 

    static void TransformComponent_SetTranslation( UUID entityID, DirectX::XMFLOAT3 *pTranslation )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        entity.GetComponent< scene::TransformComponent >().Translation = *pTranslation;
    }

    static void RigidbodyComponent_AddForce( UUID entityID, DirectX::XMFLOAT3 *pForce, bool autoAwake )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );

        Ref< physics::PhysicsActor > pPhysicsActor = physics::PhysicsEngine::GetActorOfEntity( entity );
        if ( !pPhysicsActor )
        {
            SM_LOG_ERROR( "ScriptGlue::RigidbodyComponent_AddForce > Physics actor not found" );
            return;
        }

        pPhysicsActor->AddForce( *pForce, autoAwake );
    } 

    static bool Input_IsKeyPressed(input::KeyCode keyCode)
    {
        return input::Input::IsKeyPressed( keyCode );
    }

    template < typename... ComponentType >
    static void RegisterComponentType()
    {
        (
            []()
            {
                std::string_view structName = stl::TypeNameOf< ComponentType, true >();
                std::string managedTypeName = "Smile." + std::string{ structName };

                MonoType *pManagedType =
                    mono_reflection_type_from_name( managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage() );

                if ( !pManagedType )
                {
                    SM_LOG_ERROR( "ScriptGlue::RegisterComponentType > Managed type not found in assembly: %s",
                        managedTypeName.c_str() );
                    return;
                }

                s_EntityHasComponentFuncs[pManagedType] = []( scene::Entity entity )
                { return entity.HasComponent< ComponentType >(); };
            }(),
            ... );
    }

    template < typename... ComponentType >
    static void RegisterComponentType( scene::ComponentGroup< ComponentType... > )
    {
        RegisterComponentType< ComponentType... >();
    }

    void ScriptGlue::RegisterComponentTypes()
    {
        RegisterComponentType( scene::AllComponents{} );
    }

    void ScriptGlue::RegisterFunctions()
    {
        SM_ADD_INTERNAL_CALL( Entity_HasComponent );

        SM_ADD_INTERNAL_CALL( TransformComponent_GetTranslation );
        SM_ADD_INTERNAL_CALL( TransformComponent_SetTranslation );

        SM_ADD_INTERNAL_CALL( RigidbodyComponent_AddForce );

        SM_ADD_INTERNAL_CALL( Input_IsKeyPressed );
    }
}