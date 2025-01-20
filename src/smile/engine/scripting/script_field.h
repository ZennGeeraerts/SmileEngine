/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <string>

extern "C"
{
    typedef struct _MonoClassField MonoClassField;
}

namespace smile::scripting
{
    enum class ScriptFieldType
    {
        None = 0,
        Float,
        Double,
        Bool,
        Char,
        Byte,
        Short,
        Int,
        Long,
        UByte,
        UShort,
        UInt,
        ULong,
        Vector2,
        Vector3,
        Vector4,
        Entity
    };

    struct ScriptField final
    {
        ScriptFieldType Type;
        std::string Name;
        MonoClassField *pClassField;
    };
}