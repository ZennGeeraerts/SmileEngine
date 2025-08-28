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
                SM_ASSERT_MSG( false, "ShaderDataTypeSize > Unknown ShaderDataType" );
                return 0;
        }
    }

    static ShaderDataType ShaderDataTypeFromString( const std::string &type )
    {
        if ( type == "Float" )
            return ShaderDataType::Float;
        else if ( type == "Float2" )
            return ShaderDataType::Float2;
        else if ( type == "Float3" )
            return ShaderDataType::Float3;
        else if ( type == "Float4" )
            return ShaderDataType::Float4;
        else if ( type == "Mat3" )
            return ShaderDataType::Mat3;
        else if ( type == "Mat4" )
            return ShaderDataType::Mat4;
        else if ( type == "Int" )
            return ShaderDataType::Int;
        else if ( type == "Int2" )
            return ShaderDataType::Int2;
        else if ( type == "Int3" )
            return ShaderDataType::Int3;
        else if ( type == "Int4" )
            return ShaderDataType::Int4;
        else if ( type == "Bool" )
            return ShaderDataType::Bool;
        else if ( type == "Mat4Array" )
            return ShaderDataType::Mat4Array;
        else if ( type == "Texture" )
            return ShaderDataType::Texture;
        else if ( type == "Texture2D" )
            return ShaderDataType::Texture2D;
        else if ( type == "Texture3D" )
            return ShaderDataType::Texture3D;
        else if ( type == "TextureCube" )
            return ShaderDataType::TextureCube;
        else if ( type == "String" )
            return ShaderDataType::String;
        else if ( type == "Sampler" )
            return ShaderDataType::Sampler;

        return ShaderDataType::None;
    }
}