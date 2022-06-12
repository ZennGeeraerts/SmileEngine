#include "smpch.h"
#include "smile_engine/renderer/shader/shader_reflection.h"
#include "directx11_shader.h"

#include <d3dx11effect.h>

namespace smile::renderer::utils
{
    static ShaderDataType convertDirectXShaderVariableType( D3DX11_EFFECT_TYPE_DESC effect_type_descriptor )
    {
        switch ( effect_type_descriptor.Class )
        {
            case D3D_SVC_SCALAR:
            {
                switch ( effect_type_descriptor.Type )
                {
                    case D3D_SVT_FLOAT:
                        return ShaderDataType::Float;
                    case D3D_SVT_BOOL:
                        return ShaderDataType::Bool;
                    case D3D_SVT_INT:
                        return ShaderDataType::Int;
                    default:
                        return ShaderDataType::Float;
                }
            }
            case D3D_SVC_VECTOR:
            {
                switch ( effect_type_descriptor.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        switch ( effect_type_descriptor.Columns )
                        {
                            case 2:
                                return ShaderDataType::Float2;
                            case 3:
                                return ShaderDataType::Float3;
                            default:
                                return ShaderDataType::Float3;
                        }
                    }
                    default:
                        return ShaderDataType::Float;
                }
            }
            case D3D_SVC_MATRIX_COLUMNS:
            {
                switch ( effect_type_descriptor.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        if ( ( effect_type_descriptor.Columns == 4 ) && ( effect_type_descriptor.Rows == 4 ) )
                        {
                            if ( effect_type_descriptor.Elements == 0 )
                                return ShaderDataType::Mat4;
                            else
                                return ShaderDataType::Mat4Array;
                        }

                        return ShaderDataType::Mat4;
                    }
                    default:
                        return ShaderDataType::Float;
                }
            }
            case D3D_SVC_OBJECT:
            {
                switch ( effect_type_descriptor.Type )
                {
                    case D3D_SVT_TEXTURE:
                        return ShaderDataType::Texture;
                    case D3D_SVT_TEXTURE2D:
                        return ShaderDataType::Texture2D;
                    case D3D_SVT_TEXTURE3D:
                        return ShaderDataType::Texture3D;
                    case D3D_SVT_TEXTURECUBE:
                        return ShaderDataType::TextureCube;
                    default:
                        return ShaderDataType::Texture;
                }
            }
            default:
                return ShaderDataType::Float;
        }
    }

    std::vector< ShaderVariable > reflectShaderVariables( const Ref< Shader > &shader )
    {
        DirectX11Shader *directX11_shader = static_cast< DirectX11Shader * >( shader.get() );
        std::vector< ShaderVariable > shader_variables{};

        ID3DX11Effect *effect = directX11_shader->getEffect();
        D3DX11_EFFECT_DESC effect_desc{};
        effect->GetDesc( &effect_desc );
        for ( Uint32 i{}; i < effect_desc.GlobalVariables; ++i )
        {
            ID3DX11EffectVariable *effect_variable = effect->GetVariableByIndex( i );
            SM_ASSERT( effect_variable, "reflectShaderVariables > Invalid shader variable" );

            ID3DX11EffectType *effect_type = effect_variable->GetType();
            D3DX11_EFFECT_TYPE_DESC effect_type_descriptor{};
            effect_type->GetDesc( &effect_type_descriptor );

            D3DX11_EFFECT_VARIABLE_DESC effect_variable_desc{};
            effect_variable->GetDesc( &effect_variable_desc );

            if ( effect_variable_desc.Semantic )
            {
                ShaderVariable shader_variable{};
                shader_variable.semantic = effect_variable_desc.Semantic;
                shader_variable.type = convertDirectXShaderVariableType( effect_type_descriptor );
                shader_variables.push_back( shader_variable );
            }
        }

        return shader_variables;
    }
}