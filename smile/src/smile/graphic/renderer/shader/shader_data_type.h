/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    enum class ShaderDataType : Uint8
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool,
        Mat4Array,
        Texture,
        Texture2D,
        Texture3D,
        TextureCube,
        String,
        Sampler
    };

    static Uint32 ShaderDataTypeSize( ShaderDataType type )
    {
        switch ( type )
        {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 8;
            case ShaderDataType::Float3:
                return 12;
            case ShaderDataType::Float4:
                return 16;
            case ShaderDataType::Mat3:
                return 4 * 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 8;
            case ShaderDataType::Int3:
                return 12;
            case ShaderDataType::Int4:
                return 16;
            case ShaderDataType::Bool:
                return 1;
            default:
                SM_ASSERT( false, "ShaderDataTypeSize > Unknown ShaderDataType" );
                return 0;
        }
    }
}