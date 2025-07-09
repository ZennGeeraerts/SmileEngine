/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "constant_type.h"

namespace smile::graphic
{
    Count GetSize( ConstantType type )
    {
        switch ( type )
        {
            case ConstantType::Unknown:
                return 0;
            case ConstantType::Float:
                return 4;
            case ConstantType::Float2:
                return 8;
            case ConstantType::Float3:
                return 12;
            case ConstantType::Int:
                return 4;
            case ConstantType::Bool:
                return 1;
            case ConstantType::Mat4:
                return 4 * 4 * sizeof( float );
            default:
                SM_ASSERT_MSG( false, "Invalid constant type" );
        }

        return 0;
    }
}