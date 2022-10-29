#include "smpch.h"
#include "script_glue.h"

#include <mono/metadata/object.h>

namespace Smile::Scripting
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

    void ScriptGlue::RegisterFunctions()
    {
        SM_ADD_INTERNAL_CALL( NativeLog );
        SM_ADD_INTERNAL_CALL( NativeLogVector );
    }
}