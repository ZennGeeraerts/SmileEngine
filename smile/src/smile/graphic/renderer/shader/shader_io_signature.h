/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_data_type.h"

namespace smile::graphic
{
    struct ShaderIOParameter final
    {
        ShaderIOParameter() = default;
        ShaderIOParameter( const std::string &semanticName, Uint32 semanticIndex, ShaderDataType dataType, Uint32 reg )
            : SemanticName{ semanticName }, SemanticIndex{ semanticIndex }, DataType{ dataType }, Register{ reg }
        {
        }

        std::string SemanticName;
        Uint32 SemanticIndex;
        ShaderDataType DataType;
        Uint32 Register;
    };

    class ShaderIOSignature final
    {
      public:
        ShaderIOSignature()
        {
        }

        ShaderIOSignature( const std::initializer_list< ShaderIOParameter > &parameters ) : m_Parameters{ parameters }
        {
        }

        inline const std::vector< ShaderIOParameter > &GetParameters() const
        {
            return m_Parameters;
        }

        std::vector< ShaderIOParameter >::iterator begin()
        {
            return m_Parameters.begin();
        }

        std::vector< ShaderIOParameter >::iterator end()
        {
            return m_Parameters.end();
        }

        std::vector< ShaderIOParameter >::const_iterator begin() const
        {
            return m_Parameters.cbegin();
        }

        std::vector< ShaderIOParameter >::const_iterator end() const
        {
            return m_Parameters.cend();
        }

        void AddParameter( const ShaderIOParameter &parameter )
        {
            m_Parameters.push_back( parameter );
        }

      private:
        std::vector< ShaderIOParameter > m_Parameters;
    };
}