#include "smpch.h"
#include "script_glue.h"

#include "script_engine.h"

#include "smile_engine/input/key_codes.h"
#include "smile_engine/input/input.h"

#include <mono/metadata/object.h>

namespace smile::scripting
{
#define SM_ADD_INTERNAL_CALL( name ) mono_add_internal_call( "Smile.InternalCalls::" #name, name )

    static void NativeLog( MonoString *pText, int value )
    {
        char *cStr = mono_string_to_utf8( pText );
        std::string str{ cStr };

        mono_free( cStr );
        std::cout << str << ", " << value << std::endl;
    }

    static void NativeLogVector( DirectX::XMFLOAT3 *pParameter )
    {
        DirectX::XMFLOAT3 vec = *pParameter;
        std::cout << "Value: " << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
    }

    static void GetEntityTranslation( UUID entityID, DirectX::XMFLOAT3 *pOutTranslation )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        *pOutTranslation = entity.GetComponent< scene::TransformComponent >().Translation;
    } 

    static void SetEntityTranslation( UUID entityID, DirectX::XMFLOAT3 *pTranslation )
    {
        scene::Scene *pScene = ScriptEngine::GetSceneContext();
        scene::Entity entity = pScene->GetEntityByUUID( entityID );
        entity.GetComponent< scene::TransformComponent >().Translation = *pTranslation;
    }

    static bool Input_IsKeyPressed(input::KeyCode keyCode)
    {
        return input::Input::IsKeyPressed( keyCode );
    }

    void ScriptGlue::RegisterFunctions()
    {
        SM_ADD_INTERNAL_CALL( NativeLog );
        SM_ADD_INTERNAL_CALL( NativeLogVector );

        SM_ADD_INTERNAL_CALL( GetEntityTranslation );
        SM_ADD_INTERNAL_CALL( SetEntityTranslation );

        SM_ADD_INTERNAL_CALL( Input_IsKeyPressed );
    }
}