/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/std_string.h"
#include "smile/core/fs/path.h"
#include "yaml.h"

namespace YAML
{
    template <>
    struct convert< smile::primitive::String >
    {
        static Node encode( const smile::primitive::String &value )
        {
            if ( value.GetCharCount() > 0 )
            {
                return Node{ value.GetData() };
            }
            else
            {
                return Node{ "" };
            }
        }

        static bool decode( const Node &node, smile::primitive::String &value )
        {
            if ( !node.IsScalar() )
                return false;

            value = smile::primitive::StdString{ node.as< std::string >() };
            return true;
        }
    };

    template <>
    struct convert< smile::primitive::StringView >
    {
        static Node encode( const smile::primitive::StringView value )
        {
            if ( value.GetCharCount() > 0 )
            {
                return Node{ value.GetData() };
            }
            else
            {
                return Node{ "" };
            }
        }
    };

    template <>
    struct convert< smile::fs::Path >
    {
        static Node encode( const smile::fs::Path &value )
        {
            if ( value.GetCharCount() > 0 )
            {
                return Node{ value.GetData() };
            }
            else
            {
                return Node{ "" };
            }
        }

        static bool decode( const Node &node, smile::fs::Path &value )
        {
            if ( !node.IsScalar() )
                return false;

            value = smile::primitive::StdString{ node.as< std::string >() };
            return true;
        }
    };

    inline Emitter &operator<<( Emitter &output, const smile::primitive::String &value )
    {
        if ( value.GetCharCount() > 0 )
        {
            output << value.GetData();
        }
        else
        {
            output << "";
        }

        return output;
    }

    inline Emitter &operator<<( Emitter &output, const smile::primitive::StringView value )
    {
        if ( value.GetCharCount() > 0 )
        {
            output << value.GetData();
        }
        else
        {
            output << "";
        }

        return output;
    }

    inline Emitter &operator<<( Emitter &output, const smile::fs::Path &value )
    {
        if ( value.GetCharCount() > 0 )
        {
            output << value.GetData();
        }
        else
        {
            output << "";
        }

        return output;
    }
}