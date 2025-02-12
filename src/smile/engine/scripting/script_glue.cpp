/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "script_glue.h"

#include "script_engine.h"

#include "world/components.h"

#include "input/key_codes.h"
#include "input/input.h"

#include "engine/physics/physics_engine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <string_view>

namespace smile::scripting
{
    static std::unordered_map< MonoType *, std::function< bool( world::Entity ) > > s_EntityHasComponentFuncs{};

#define SM_ADD_INTERNAL_CALL( name ) mono_add_internal_call( "Smile.InternalCalls::" #name, name )

    static bool Entity_HasComponent( primitive::UUID entityID, MonoReflectionType *pComponentType )
    {
        world::World *pWorld = ScriptEngine::GetWorldContext();
        SM_ASSERT( pWorld, "" );
        world::Entity entity = pWorld->GetEntityByUUID( entityID );
        SM_ASSERT( entity, "" );

        MonoType *pManagedType = mono_reflection_type_get_type( pComponentType );
        SM_ASSERT( s_EntityHasComponentFuncs.find( pManagedType ) != s_EntityHasComponentFuncs.end(),
            "ScriptGlue::Entity_HasComponent > UUnregistered type" );

        return s_EntityHasComponentFuncs.at( pManagedType )( entity );
    }

    static void TransformComponent_GetTranslation( primitive::UUID entityID, DirectX::XMFLOAT3 *pOutTranslation )
    {
        world::World *pWorld = ScriptEngine::GetWorldContext();
        world::Entity entity = pWorld->GetEntityByUUID( entityID );
        *pOutTranslation = entity.GetComponent< world::ecs::TransformComponent >().Translation;
    }

    static void TransformComponent_SetTranslation( primitive::UUID entityID, DirectX::XMFLOAT3 *pTranslation )
    {
        world::World *pWorld = ScriptEngine::GetWorldContext();
        world::Entity entity = pWorld->GetEntityByUUID( entityID );
        auto &transformComponent = entity.GetComponent< world::ecs::TransformComponent >();

        transformComponent.Translation = *pTranslation;
        transformComponent.TransformChanged |=
            static_cast< Uint32 >( world::ecs::TransformComponent::TransformChanged::Translation );
    }

    static void RigidbodyComponent_AddForce( primitive::UUID entityID, DirectX::XMFLOAT3 *pForce, bool autoAwake )
    {
        world::World *pWorld = ScriptEngine::GetWorldContext();
        pWorld->AddForce( entityID, *pForce, autoAwake );
    }

    static void
    CharacterControllerComponent_Move( primitive::UUID entityID, DirectX::XMFLOAT3 *pDisplacement, float minDist )
    {
        world::World *pWorld = ScriptEngine::GetWorldContext();
        pWorld->MoveCharacterController( entityID, *pDisplacement, minDist );
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
                std::string_view structName = foundation::TypeNameOf< ComponentType, true >();
                std::string managedTypeName = "Smile." + std::string{ structName };

                MonoType *pManagedType =
                    mono_reflection_type_from_name( managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage() );

                if ( !pManagedType )
                {
                    SM_LOG_ERROR(
                        "ScriptGlue::RegisterComponentType > Managed type not found in assembly: {}", managedTypeName );
                    return;
                }

                s_EntityHasComponentFuncs[pManagedType] = []( world::Entity entity )
                { return entity.HasComponent< ComponentType >(); };
            }(),
            ... );
    }

    template < typename... ComponentType >
    static void RegisterComponentType( world::ComponentGroup< ComponentType... > )
    {
        RegisterComponentType< ComponentType... >();
    }

    void ScriptGlue::RegisterComponentTypes()
    {
        RegisterComponentType( world::AllComponents{} );
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