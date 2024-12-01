/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "script_glue.h"

#include "script_engine.h"

#include "smile_engine/core/scene/components.h"

#include "smile_engine/core/input/key_codes.h"
#include "smile_engine/core/input/input.h"

#include "smile_engine/physics/physics_engine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <string_view>

namespace smile::scripting
{
    static std::unordered_map< MonoType *, std::function< bool( scene::Entity ) > > s_EntityHasComponentFuncs{};

#define SM_ADD_INTERNAL_CALL( name ) mono_add_internal_call( "Smile.InternalCalls::" #name, name )

    static bool Entity_HasComponent( primitive::UUID entityID, MonoReflectionType *pComponentType )
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

    static void TransformComponent_GetTranslation( primitive::UUID entityID, DirectX::XMFLOAT3 *pOutTranslation )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        *pOutTranslation = entity.GetComponent< scene::ecs::TransformComponent >().Translation;
    }

    static void TransformComponent_SetTranslation( primitive::UUID entityID, DirectX::XMFLOAT3 *pTranslation )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        auto &transformComponent = entity.GetComponent< scene::ecs::TransformComponent >();

        transformComponent.Translation = *pTranslation;
        transformComponent.TransformChanged |=
            static_cast< Uint32 >( scene::ecs::TransformComponent::TransformChanged::Translation );
    }

    static void RigidbodyComponent_AddForce( primitive::UUID entityID, DirectX::XMFLOAT3 *pForce, bool autoAwake )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();

        const auto &physicsSystem = pScene->GetPhysicsSystem();
        Ref< physics::Rigidbody > pRigidbody = physicsSystem.GetRigidbody( entityID );
        if ( !pRigidbody )
        {
            SM_LOG_ERROR( "ScriptGlue::RigidbodyComponent_AddForce > Physics actor not found" );
            return;
        }

        pRigidbody->AddForce( *pForce, autoAwake );
    }

    static void
    CharacterControllerComponent_Move( primitive::UUID entityID, DirectX::XMFLOAT3 *pDisplacement, float minDist )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();

        const auto &physicsSystem = pScene->GetPhysicsSystem();
        Ref< physics::CharacterController > pCharacterController = physicsSystem.GetCharacterController( entityID );
        if ( !pCharacterController )
        {
            SM_LOG_ERROR( "ScriptGlue::CharacterControllerComponent_Move > Character controller not found" );
            return;
        }

        pCharacterController->Move( *pDisplacement, minDist );
    }

    static bool Input_IsKeyPressed( input::KeyCode keyCode )
    {
        return input::Input::IsKeyPressed( keyCode );
    }

    template < typename... ComponentType >
    static void RegisterComponentType()
    {
        (
            []()
            {
                std::string_view structName = compiled::TypeNameOf< ComponentType, true >();
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

        SM_ADD_INTERNAL_CALL( CharacterControllerComponent_Move );

        SM_ADD_INTERNAL_CALL( Input_IsKeyPressed );
    }
}