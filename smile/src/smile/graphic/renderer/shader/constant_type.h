/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string_view.h"

namespace smile::graphic
{
    enum class ConstantType
    {
        Unknown,
        Float,
        Float2,
        Float3,
        Int,
        Bool,
        Mat4,
        Mat4Array,
        Count
    };

    struct ConstantTypeInfo final
    {
        ConstantType Type;
        Count Size;
        primitive::StringView Name;
    };

    const ConstantTypeInfo &GetConstantTypeInfo( ConstantType type );
    const ConstantTypeInfo &GetConstantTypeInfo( const primitive::StringView constantView );
}