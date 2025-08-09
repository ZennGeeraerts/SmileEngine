/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
    template < typename Type >
    struct convert< smile::primitive::Vector< Type > >
    {
        static Node encode( const smile::primitive::Vector< Type > &values )
        {
            Node node{};

            for ( const auto &value : values )
            {
                node.push_back( value );
            }

            return node;
        }

        static bool decode( const Node &node, smile::primitive::Vector< Type > &values )
        {
            if ( !node.IsSequence() )
            {
                return false;
            }

            for ( const auto &value : node )
            {
                values.PushBack( value.as< Type >() );
            }

            return true;
        }
    };
}