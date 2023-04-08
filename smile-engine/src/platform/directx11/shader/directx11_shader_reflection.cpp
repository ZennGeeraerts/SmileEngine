#include "smpch.h"
#include "smile_engine/graphic/shader/shader_reflection.h"
#include "directx11_shader.h"

#include <d3dx11effect.h>

namespace smile::graphic::utils
{
    static ShaderDataType ConvertDirectXShaderVariableType( D3DX11_EFFECT_TYPE_DESC effectTypeDescriptor )
    {
        switch ( effectTypeDescriptor.Class )
        {
            case D3D_SVC_SCALAR:
            {
                switch ( effectTypeDescriptor.Type )
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
                switch ( effectTypeDescriptor.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        switch ( effectTypeDescriptor.Columns )
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
                switch ( effectTypeDescriptor.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        if ( ( effectTypeDescriptor.Columns == 4 ) && ( effectTypeDescriptor.Rows == 4 ) )
                        {
                            if ( effectTypeDescriptor.Elements == 0 )
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
                switch ( effectTypeDescriptor.Type )
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

    std::vector< ShaderVariable > ReflectShaderVariables( const Ref< Shader > &pShader )
    {
        DirectX11Shader *pDirectX11Shader = static_cast< DirectX11Shader * >( pShader.get() );
        std::vector< ShaderVariable > shaderVariables{};

        ID3DX11Effect *pEffect = pDirectX11Shader->pEffect;
        D3DX11_EFFECT_DESC effectDesc{};
        pEffect->GetDesc( &effectDesc );
        for ( Uint32 i{}; i < effectDesc.GlobalVariables; ++i )
        {
            ID3DX11EffectVariable *pEffectVariable = pEffect->GetVariableByIndex( i );
            SM_ASSERT( pEffectVariable, "ReflectShaderVariables > Invalid shader variable" );

            ID3DX11EffectType *pEffectType = pEffectVariable->GetType();
            D3DX11_EFFECT_TYPE_DESC effectTypeDescriptor{};
            pEffectType->GetDesc( &effectTypeDescriptor );

            D3DX11_EFFECT_VARIABLE_DESC effectVariableDesc{};
            pEffectVariable->GetDesc( &effectVariableDesc );

            if ( effectVariableDesc.Semantic )
            {
                ShaderVariable shaderVariable{};
                shaderVariable.Semantic = effectVariableDesc.Semantic;
                shaderVariable.Type = ConvertDirectXShaderVariableType( effectTypeDescriptor );
                shaderVariables.push_back( shaderVariable );
            }
        }

        return shaderVariables;
    }
}