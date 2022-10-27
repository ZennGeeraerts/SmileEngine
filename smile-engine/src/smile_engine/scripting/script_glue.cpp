#include "smpch.h"
#include "script_glue.h"

#include <mono/metadata/object.h>

namespace smile::scripting
{
#define SM_ADD_INTERNAL_CALL( name ) mono_add_internal_call( "Smile.InternalCalls::" #name, name )

    static void nativeLog( MonoString *text, int value )
    {
        char *c_str = mono_string_to_utf8( text );
        std::string str{ c_str };

        mono_free( c_str );
        std::cout << str << ", " << value << std::endl;
    }

    static void nativeLogVector( DirectX::XMFLOAT3 *parameter )
    {
        DirectX::XMFLOAT3 vec = *parameter;
        std::cout << "Value: " << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
    } 

    void ScriptGlue::registerFunctions()
    {
        SM_ADD_INTERNAL_CALL( nativeLog );
        SM_ADD_INTERNAL_CALL( nativeLogVector );
    }
}