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

    Material::Material()
    {
        /*for ( const auto &binding : pMaterialAsset->GetBindings() )
        {
            switch ( binding.Value.Type )
            {
                case rhi::ResourceType::Texture_SRV:
                case rhi::ResourceType::Texture_UAV:
                {
                    SM_ASSERT( !m_Params.HasItemAtKey( binding.Key ) );

                    MaterialParam param{ binding.Key, MaterialParamType::Texture, Texture::Ref{} };
                    m_Params.Insert( binding.Key, std::move( param ) );
                    break;
                }

                case rhi::ResourceType::Sampler:
                {
                    SM_ASSERT( !m_Params.HasItemAtKey( binding.Key ) );

                    MaterialParam param{ binding.Key, MaterialParamType::Sampler, Sampler::Ref{} };
                    m_Params.Insert( binding.Key, std::move( param ) );
                    break;
                }

                case rhi::ResourceType::ConstantBuffer:
                {
                    const auto &constantBufferDesc = pMaterialAsset->GetConstantBufferDesc( binding.Key );

                    m_ConstantBufferData.Insert( binding.Key, ConstantBufferData( constantBufferDesc.GetSize() ) );

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
        }*/
    }

    void Material::Clear()
    {
        m_Params.Clear();
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
            SetConstantBufferData( m_VertexShaderCB, name, data );
            SetConstantBufferData( m_PixelShaderCB, name, data );
        }
        else
        {
            rhi::BindingSetDescriptor desc{};
            std::find_if( desc.begin(), desc.end(), []( const auto &item ) {} );
        }
    }

    void Material::SetConstantBufferData( ConstantBuffer::Ref constantBuffer,
        const primitive::StringView name,
        const MaterialParamValue &data )
    {
        const auto &desc = constantBuffer->GetDescriptor();
        auto it = std::find_if( desc.begin(),
            desc.end(),
            [name]( const auto &constantBufferItem ) { return constantBufferItem.Name == name; } );

        if ( it != desc.end() )
        {
            const auto &cbItem = ( *it );
            const auto &valueData = std::get< primitive::Vector< Byte > >( data );

            SM_ASSERT( cbItem.GetStride() == valueData.GetItemCount() );

            memory::CopyArrayItems( reinterpret_cast< Byte * >( constantBuffer->GetBuffer() ) + cbItem.Offset,
                cbItem.GetStride(),
                valueData.GetData() );
        }
    }

    void Material::SetBindingSetData( BindingSet::Ref bindingSet,
        const primitive::StringView name,
        const MaterialParamValue &data )
    {
        
    }

    const MaterialParam &Material::GetParam( const primitive::StringView name ) const
    {
        return m_Params.GetItemAtKey( name );
    }
}