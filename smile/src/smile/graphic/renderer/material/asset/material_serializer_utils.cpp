/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        material_serializer_utils.cpp
 * @author      Zenn Geeraerts
 * @created     5 May 2026
 * @brief       Utility functions for serializing material assets to a file
 */
#include "smpch.h"
#include "material_serializer_utils.h"

#include "smile/core/yaml/string.h"
#include "smile/core/yaml/math.h"
#include "smile/core/asset/asset_manager.h"
#include "smile/graphic/renderer/sprite/texture_asset.h"

namespace smile::graphic
{
    void SerializeMaterialAssetDescriptor( const MaterialLayout &layout,
        const MaterialAssetDescriptor &desc,
        yaml::Emitter &output )
    {
        output << YAML::Key << "Parameters";

        output << YAML::BeginMap;

        for ( const auto &param : layout.Parameters )
        {
            auto it = desc.Parameters.FindItemAtKey( param.Name );
            if ( it != desc.Parameters.end() )
            {
                switch ( param.Type )
                {
                    case MaterialParameterType::Bool:
                        output << YAML::Key << param.Name << YAML::Value << std::get< bool >( it.GetItem() );
                        break;
                    case MaterialParameterType::Int:
                        output << YAML::Key << param.Name << YAML::Value << std::get< int >( it.GetItem() );
                        break;
                    case MaterialParameterType::Float:
                        output << YAML::Key << param.Name << YAML::Value << std::get< float >( it.GetItem() );
                        break;
                    case MaterialParameterType::Float2:
                        output << YAML::Key << param.Name << YAML::Value
                               << std::get< DirectX::XMFLOAT2 >( it.GetItem() );
                        break;
                    case MaterialParameterType::Float3:
                        output << YAML::Key << param.Name << YAML::Value
                               << std::get< DirectX::XMFLOAT3 >( it.GetItem() );
                        break;
                }
            }
        }

        output << YAML::EndMap;

        output << YAML::Key << "Textures";

        output << YAML::BeginMap;

        for ( const auto &texture : layout.Textures )
        {
            auto it = desc.TextureBindings.FindItemAtKey( texture.Name );
            if ( it != desc.TextureBindings.end() )
            {
                const MaterialAssetTextureBinding &binding = it.GetItem();
                if ( !binding.Texture )
                    continue;

                output << YAML::Key << texture.Name << YAML::Value;

                output << YAML::BeginMap;
                {
                    output << YAML::Key << "Texture" << YAML::Value << binding.Texture->m_Handle;

                    output << YAML::Key << "Sampler";
                    output << YAML::BeginMap;
                    {
                        output << YAML::Key << "Filtering" << YAML::Value
                               << static_cast< Uint32 >( binding.SamplerDescriptor.Filtering );

                        output << YAML::Key << "AddressingU" << YAML::Value
                               << static_cast< Uint32 >( binding.SamplerDescriptor.AddressingU );

                        output << YAML::Key << "AddressingV" << YAML::Value
                               << static_cast< Uint32 >( binding.SamplerDescriptor.AddressingV );

                        output << YAML::Key << "AddressingW" << YAML::Value
                               << static_cast< Uint32 >( binding.SamplerDescriptor.AddressingW );
                    }
                    output << YAML::EndMap;
                }
                output << YAML::EndMap;
            }
        }

        output << YAML::EndMap;
    }

    void DeserializeMaterialAssetDescriptor( const yaml::Node &node,
        const MaterialLayout &layout,
        MaterialAssetDescriptor &desc )
    {
        if ( node["Parameters"] )
        {
            for ( const auto &param : layout.Parameters )
            {
                YAML::Node valueNode = node["Parameters"][param.Name];
                if ( !valueNode )
                    continue;

                switch ( param.Type )
                {
                    case MaterialParameterType::Bool:
                    {
                        desc.SetParameter( param.Name, valueNode.as< bool >() );
                        break;
                    }

                    case MaterialParameterType::Int:
                    {
                        desc.SetParameter( param.Name, valueNode.as< int >() );
                        break;
                    }

                    case MaterialParameterType::Float:
                    {
                        desc.SetParameter( param.Name, valueNode.as< float >() );
                        break;
                    }

                    case MaterialParameterType::Float2:
                    {
                        desc.SetParameter( param.Name, valueNode.as< DirectX::XMFLOAT2 >() );
                        break;
                    }

                    case MaterialParameterType::Float3:
                    {
                        desc.SetParameter( param.Name, valueNode.as< DirectX::XMFLOAT3 >() );
                        break;
                    }
                }
            }
        }

        if ( node["Textures"] )
        {
            for ( const auto &texture : layout.Textures )
            {
                YAML::Node valueNode = node["Textures"][texture.Name];
                if ( !valueNode )
                    continue;

                if ( !valueNode["Texture"] || !valueNode["Sampler"] )
                    continue;

                asset::AssetHandle textureAssetHandle = valueNode["Texture"].as< Uint64 >();

                rhi::SamplerDescriptor samplerDesc{};

                samplerDesc.Filtering =
                    static_cast< rhi::SamplerFiltering >( valueNode["Sampler"]["Filtering"].as< Uint32 >() );

                samplerDesc.AddressingU =
                    static_cast< rhi::SamplerAddressing >( valueNode["Sampler"]["AddressingU"].as< Uint32 >() );

                samplerDesc.AddressingV =
                    static_cast< rhi::SamplerAddressing >( valueNode["Sampler"]["AddressingV"].as< Uint32 >() );

                samplerDesc.AddressingW =
                    static_cast< rhi::SamplerAddressing >( valueNode["Sampler"]["AddressingW"].as< Uint32 >() );

                TextureAsset::Ref textureAsset = asset::AssetManager::GetAsset< TextureAsset >( textureAssetHandle );

                desc.SetTextureBinding( texture.Name, textureAsset, samplerDesc );
            }
        }
    }
}