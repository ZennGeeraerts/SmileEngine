/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "constant_type.h"

namespace smile::graphic
{
    static const ConstantTypeInfo s_ConstantTypeInfo[] = { { ConstantType::Unknown, 0, "Unknown" },
        { ConstantType::Float, 4, "Float" },
        { ConstantType::Float2, 8, "Float2" },
        { ConstantType::Float3, 12, "Float3" },
        { ConstantType::Int, 4, "Int" },
        { ConstantType::Uint, 4, "Uint" },
        { ConstantType::Bool, 1, "Bool" },
        { ConstantType::Mat4, 4 * 4 * sizeof( float ), "Mat4" },
        { ConstantType::Mat4Array, 4 * 4 * sizeof( float ), "Mat4Array" } };

    const ConstantTypeInfo &GetConstantTypeInfo( ConstantType type )
    {
        static_assert(
            sizeof( s_ConstantTypeInfo ) / sizeof( ConstantTypeInfo ) == static_cast< size_t >( ConstantType::Count ) );

        if ( static_cast< Uint32 >( type ) >= static_cast< Uint32 >( ConstantType::Count ) )
            return s_ConstantTypeInfo[0]; // Unknown constant type

        const ConstantTypeInfo &info = s_ConstantTypeInfo[static_cast< Uint32 >( type )];

        SM_ASSERT_MSG( info.Type == type, "Constant type mismatch" );

        return info;
    }

    const ConstantTypeInfo &GetConstantTypeInfo( const primitive::StringView constantView )
    {
        for ( const ConstantTypeInfo &info : s_ConstantTypeInfo )
        {
            if ( constantView == info.Name )
            {
                return info;
            }
        }

        return s_ConstantTypeInfo[0]; // Unknown constant type
    }
}