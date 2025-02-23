/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_shader.h"

namespace smile::graphic
{
    DirectX11Shader::~DirectX11Shader()
    {
        for ( auto &effectVar : EffectVariableMap )
        {
            SAFE_RELEASE( effectVar.second );
        }

        EffectVariableMap.clear();

        SAFE_RELEASE( pInputLayout );
        SAFE_RELEASE( pTechnique );
        SAFE_RELEASE( pEffect );
    }

    void DirectX11Shader::UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix )
    {
        auto pMatrixVariable = GetEffectVariable( sementicName )->AsMatrix();
        if ( pMatrixVariable->IsValid() )
        {
            pMatrixVariable->SetMatrix( &matrix._11 );
        }
    }

    void DirectX11Shader::UploadMat4Array( const std::string &sementicName,
        const std::vector< DirectX::XMFLOAT4X4 > &matArray )
    {
        auto pMatArrayVariable = GetEffectVariable( sementicName )->AsMatrix();
        if ( pMatArrayVariable->IsValid() )
        {
            pMatArrayVariable->SetMatrixArray( &matArray[0]._11, 0, static_cast< Uint32 >( matArray.size() ) );
        }
    }

    void DirectX11Shader::UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value )
    {
        auto pVectorVariable = GetEffectVariable( sementicName )->AsVector();
        if ( pVectorVariable->IsValid() )
        {
            pVectorVariable->SetFloatVector( &value.x );
        }
    }

    void DirectX11Shader::UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value )
    {
        auto pVectorVariable = GetEffectVariable( sementicName )->AsVector();
        if ( pVectorVariable->IsValid() )
        {
            pVectorVariable->SetFloatVector( &value.x );
        }
    }

    void DirectX11Shader::UploadInt( const std::string &sementicName, int value )
    {
        auto pIntVariable = GetEffectVariable( sementicName )->AsScalar();
        if ( pIntVariable->IsValid() )
        {
            pIntVariable->SetInt( value );
        }
    }

    void DirectX11Shader::UploadTexture( const std::string &sementicName, const memory::Ref< Texture > &pTexture )
    {
        auto pTextureVariable = GetEffectVariable( sementicName )->AsShaderResource();
        if ( pTextureVariable->IsValid() )
        {
            pTextureVariable->SetResource( static_cast< ID3D11ShaderResourceView * >( pTexture->GetData() ) );
        }
    }

    void DirectX11Shader::UploadBool( const std::string &sementicName, bool value )
    {
        auto pBoolVariable = GetEffectVariable( sementicName )->AsScalar();
        if ( pBoolVariable->IsValid() )
        {
            pBoolVariable->SetBool( value );
        }
    }

    void DirectX11Shader::UploadFloat( const std::string &sementicName, float value )
    {
        auto pFloatVariable = GetEffectVariable( sementicName )->AsScalar();
        if ( pFloatVariable->IsValid() )
        {
            pFloatVariable->SetFloat( value );
        }
    }

    ID3DX11EffectVariable *DirectX11Shader::GetEffectVariable( const std::string &sementicName )
    {
        if ( EffectVariableMap.find( sementicName ) != EffectVariableMap.end() )
            return EffectVariableMap[sementicName];

        auto pEffectVariable = pEffect->GetVariableBySemantic( sementicName.c_str() );
        if ( !pEffectVariable->IsValid() )
        {
            SM_LOG_WARNING( "DirectX11Shader::GetEffectVariable > Invalid effect variable: {}", sementicName );
            return nullptr;
        }

        EffectVariableMap[sementicName] = pEffectVariable;
        return pEffectVariable;
    }
}