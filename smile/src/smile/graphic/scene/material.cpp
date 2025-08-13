/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

#include "smile/common/memory/memory.h"

namespace smile::graphic
{
    static MaterialParamType ConvertConstantTypeToMaterialParamType( ConstantType constantType )
    {
        switch ( constantType )
        {
            case ConstantType::Float:
                return MaterialParamType::Float;
            case ConstantType::Float2:
                return MaterialParamType::Float2;
            case ConstantType::Float3:
                return MaterialParamType::Float3;
            case ConstantType::Int:
                return MaterialParamType::Int;
            case ConstantType::Bool:
                return MaterialParamType::Bool;
            default:
                SM_ASSERT( false, "Unsupported constant type for material system" );
        }
    }

    static bool IsConstantBufferMaterialParamType( MaterialParamType type )
    {
        switch ( type )
        {
            case MaterialParamType::Float:
            case MaterialParamType::Float2:
            case MaterialParamType::Float3:
            case MaterialParamType::Int:
            case MaterialParamType::Bool:
                return true;
            case MaterialParamType::Texture:
            case MaterialParamType::Sampler:
                return false;
            default:
                SM_ASSERT( false, "Unsupported material parameter type" );
        }
    }

    Material::Material( const ShaderAsset::Ref &pVertexShader, const ShaderAsset::Ref &pPixelShader )
        : m_BindingLayout{ { rhi::ShaderStage::Vertex, rhi::ShaderStage::Pixel } }
    {
        SetShaders( pVertexShader, pPixelShader );
    }

    Material::~Material()
    {
    }

    void Material::SetShaders( ShaderAsset::Ref pVertexShader, ShaderAsset::Ref pPixelShader )
    {
        m_pVertexShader = pVertexShader;
        m_pPixelShader = pPixelShader;

        Clear();

        auto insertResourcesFunc = [&]( const ShaderReflectionData &reflectionData )
        {
            for ( const ShaderResourceBinding &binding : reflectionData.ShaderResourceBindings )
            {
                rhi::BindingLayoutElement element{ binding.BindPoint, binding.Type };
                m_BindingLayout.AddElement( std::move( element ) );

                switch ( binding.Type )
                {
                    case rhi::ResourceType::Texture_SRV:
                    case rhi::ResourceType::Texture_UAV:
                    {
                        SM_ASSERT( !m_Params.HasItemAtKey( binding.Name ) );

                        MaterialParam param{ binding.Name, MaterialParamType::Texture, Texture::Ref{} };
                        m_Params.Insert( binding.Name, std::move( param ) );
                        break;
                    }

                    case rhi::ResourceType::Sampler:
                    {
                        SM_ASSERT( !m_Params.HasItemAtKey( binding.Name ) );

                        MaterialParam param{ binding.Name, MaterialParamType::Sampler, Sampler::Ref{} };
                        m_Params.Insert( binding.Name, std::move( param ) );
                        break;
                    }

                    case rhi::ResourceType::ConstantBuffer:
                    {
                        const auto &constantBufferDesc =
                            reflectionData.ConstantBufferDescs.GetItemAtKey( binding.Name );

                        m_ConstantBufferData.Insert( binding.Name, ConstantBufferData( constantBufferDesc.GetSize() ) );

                        for ( const auto &item : constantBufferDesc )
                        {
                            SM_ASSERT( !m_Params.HasItemAtKey( item.Name ) );

                            MaterialParam param{ item.Name,
                                ConvertConstantTypeToMaterialParamType( item.Type ),
                                primitive::Vector< Byte >( item.Size ) };

                            m_Params.Insert( item.Name, std::move( param ) );
                        }
                        break;
                    }

                    default:
                        SM_ASSERT_MSG( false, "Unsupported resource type" );
                }
            }
        };

        insertResourcesFunc( pVertexShader->GetReflectionData() );
        insertResourcesFunc( pPixelShader->GetReflectionData() );
    }

    void Material::Clear()
    {
        m_BindingLayout.Clear();
        m_Params.Clear();
        m_ConstantBufferData.Clear();
    }

    void Material::SetParam( const primitive::StringView name, const MaterialParamValue &data )
    {
        if ( !m_Params.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetParam > Could not find material parameter with name: {}", name );
            return;
        }

        auto &param = m_Params.GetItemAtKey( name );
        param.Data = data;

        if ( IsConstantBufferMaterialParamType( param.Type ) )
        {
            auto trySetValueInConstantBufferData = [&]( const ShaderReflectionData &reflectionData )
            {
                for ( const auto &constantBufferDesc : reflectionData.ConstantBufferDescs )
                {
                    auto it = std::find_if( constantBufferDesc.Value.begin(),
                        constantBufferDesc.Value.end(),
                        [name]( const auto &constantBufferItem ) { return constantBufferItem.Name == name; } );

                    if ( it != constantBufferDesc.Value.end() )
                    {
                        auto &cbData = m_ConstantBufferData.GetItemAtKey( constantBufferDesc.Key );
                        const auto &cbItem = ( *it );
                        const auto &valueData = std::get< primitive::Vector< Byte > >( data );

                        memory::CopyArrayItems(
                            cbData.GetData() + cbItem.Offset, cbItem.GetStride(), valueData.GetData() );

                        return true;
                    }
                }

                return false;
            };

            if ( trySetValueInConstantBufferData( m_pVertexShader->GetReflectionData() ) )
                return;

            trySetValueInConstantBufferData( m_pPixelShader->GetReflectionData() );
        }
    }
}