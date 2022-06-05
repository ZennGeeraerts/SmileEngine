#include "smpch.h"
#include "directx11_shader.h"

#include "smile_engine/core/application.h"
#include "platform/directX11/directx11_diagnostics.h"

#include <d3dcompiler.h>

namespace smile
{
    static DXGI_FORMAT ShaderDataTypeToDirectXBaseType( ShaderDataType type )
    {
        switch ( type )
        {
            case ShaderDataType::Float:
                return DXGI_FORMAT_R32_FLOAT;
            case ShaderDataType::Float2:
                return DXGI_FORMAT_R32G32_FLOAT;
            case ShaderDataType::Float3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case ShaderDataType::Float4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case ShaderDataType::Mat3:
                return DXGI_FORMAT_UNKNOWN;
            case ShaderDataType::Mat4:
                return DXGI_FORMAT_UNKNOWN;
            case ShaderDataType::Int:
                return DXGI_FORMAT_R32_SINT;
            case ShaderDataType::Int2:
                return DXGI_FORMAT_R32G32_SINT;
            case ShaderDataType::Int3:
                return DXGI_FORMAT_R32G32B32_SINT;
            case ShaderDataType::Int4:
                return DXGI_FORMAT_R32G32B32A32_SINT;
            case ShaderDataType::Bool:
                return DXGI_FORMAT_UNKNOWN;
            default:
                SM_ASSERT( false, "DirectX11Shader::ShaderDataTypeToDirectXBaseType > Unknown ShaderDataType" );
                return DXGI_FORMAT_UNKNOWN;
        }
    }

    static ShaderDataType DirectXBaseTypeToShaderDataType( DXGI_FORMAT type )
    {
        switch ( type )
        {
            case DXGI_FORMAT_R32_FLOAT:
                return ShaderDataType::Float;
            case DXGI_FORMAT_R32G32_FLOAT:
                return ShaderDataType::Float2;
            case DXGI_FORMAT_R32G32B32_FLOAT:
                return ShaderDataType::Float3;
            case DXGI_FORMAT_R32G32B32A32_FLOAT:
                return ShaderDataType::Float4;
            case DXGI_FORMAT_R32_SINT:
                return ShaderDataType::Int;
            case DXGI_FORMAT_R32G32_SINT:
                return ShaderDataType::Int2;
            case DXGI_FORMAT_R32G32B32_SINT:
                return ShaderDataType::Int3;
            case DXGI_FORMAT_R32G32B32A32_SINT:
                return ShaderDataType::Int4;
            default:
                SM_ASSERT( false, "DirectX11Shader::DirectXBaseTypeToShaderDataType > Unknown DXGI Type" );
                return ShaderDataType::None;
        }
    }

    DirectX11Shader::DirectX11Shader( const std::string &assetFile,
        const BufferLayout &layout,
        const std::string &techniqueName )
        : m_BufferLayout{ layout }
    {
        Initalize( assetFile, techniqueName );
        BuildInputLayout( layout );
    }

    DirectX11Shader::DirectX11Shader( const std::string &assetFile, const std::string &techniqueName )
    {
        Initalize( assetFile, techniqueName );
        BuildInputLayout();
    }

    void DirectX11Shader::Initalize( const std::string &assetFile, const std::string &techniqueName )
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pDirectX11Context, "DirectX11Shader > Rendering context is not a DirectX 11 Graphics Context" );

        if ( !LoadEffect( m_pDirectX11Context->GetDevice(), assetFile ) )
        {
            SAFE_RELEASE( m_pEffect );
            SM_ASSERT( false, "DirectX11Shader > Failed to load effect" );
        }

        if ( !techniqueName.empty() )
            m_pTechnique = m_pEffect->GetTechniqueByName( techniqueName.c_str() );
        else
            m_pTechnique = m_pEffect->GetTechniqueByIndex( 0 );

        if ( !m_pTechnique->IsValid() )
            SM_LOG_WARNING( "DirectX11Shader > Invalid technique" );

        SetName( assetFile );
    }

    void DirectX11Shader::SetName( const std::string &assetFile )
    {
        // Find name from asset path
        auto lastSlash = assetFile.find_last_of( "/\\" );
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = assetFile.rfind( '.' );
        auto count = lastDot == std::string::npos ? assetFile.size() - lastSlash : lastDot - lastSlash;
        m_Name = assetFile.substr( lastSlash, count );
    }

    DirectX11Shader::~DirectX11Shader()
    {
        for ( auto &effectVar : m_EffectVariableMap )
        {
            SAFE_RELEASE( effectVar.second );
        }

        m_EffectVariableMap.clear();

        SAFE_RELEASE( m_pInputLayout );
        SAFE_RELEASE( m_pTechnique );
        SAFE_RELEASE( m_pEffect );
    }

    void DirectX11Shader::Bind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetInputLayout( m_pInputLayout );
    }

    void DirectX11Shader::Unbind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetInputLayout( nullptr );
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

    void DirectX11Shader::UploadFloat2(const std::string& semanticName, const DirectX::XMFLOAT2& value)
    {
        auto pVectorVariable = GetEffectVariable( semanticName )->AsVector();
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

    void DirectX11Shader::UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D )
    {
        auto pTextureVariable = GetEffectVariable( sementicName )->AsShaderResource();
        if ( pTextureVariable->IsValid() )
        {
            pTextureVariable->SetResource( ( ID3D11ShaderResourceView * )pTexture2D->GetData() );
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

    bool DirectX11Shader::LoadEffect( ID3D11Device *pDevice, const std::string &assetFile )
    {
        HRESULT result{ S_OK };
        ID3D10Blob *pErrorBlob{ nullptr };

        DWORD shaderFlags{ 0 };
#if defined( DEBUG ) || defined( _DEBUG )
        shaderFlags |= D3DCOMPILE_DEBUG;
        shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        std::wstring assetFileWStr{ assetFile.begin(), assetFile.end() };
        result = D3DX11CompileEffectFromFile(
            assetFileWStr.c_str(), nullptr, nullptr, shaderFlags, 0, pDevice, &m_pEffect, &pErrorBlob );

        if ( FAILED( result ) )
        {
            if ( pErrorBlob )
            {
                char *pErrors{ ( char * )pErrorBlob->GetBufferPointer() };

                std::wstringstream ss;
                for ( unsigned int i{}; i < pErrorBlob->GetBufferSize(); ++i )
                {
                    ss << pErrors[i];
                }

                OutputDebugStringW( ss.str().c_str() );
                pErrorBlob->Release();
                pErrorBlob = nullptr;

                SM_LOG_ERROR( "%s", ss.str() );
            }
            else
            {
                SM_LOG_ERROR( "DirectX11Shader::LoadEffect > Failed to CreateEffectFromFile: %s , error: %ls",
                    assetFile.c_str(),
                    GetDirectX11ErrorMessage( result ) );
            }

            return false;
        }

        return true;
    }

    void DirectX11Shader::BuildInputLayout( const BufferLayout &layout )
    {
        std::vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
        for ( const auto &element : layout )
        {
            inputDescs.push_back( D3D11_INPUT_ELEMENT_DESC{ element.m_Name.c_str(),
                0,
                ShaderDataTypeToDirectXBaseType( element.m_DataType ),
                0,
                element.m_Offset,
                D3D11_INPUT_PER_VERTEX_DATA,
                0 } );
        }

        Uint32 count{ static_cast< Uint32 >( inputDescs.size() ) };

        D3DX11_PASS_DESC passDesc{};
        m_pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
        HRESULT result = m_pDirectX11Context->GetDevice()->CreateInputLayout(
            inputDescs.data(), count, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout );

        if ( FAILED( result ) )
            SM_LOG_ERROR( "DirectX11Shader::BuildInputLayout > Failed to create input layout: %ls",
                GetDirectX11ErrorMessage( result ) );
    }

    void DirectX11Shader::BuildInputLayout()
    {
        D3DX11_PASS_SHADER_DESC passShaderDesc{};
        m_pTechnique->GetPassByIndex( 0 )->GetVertexShaderDesc( &passShaderDesc );

        D3DX11_EFFECT_SHADER_DESC effectShaderDesc{};
        passShaderDesc.pShaderVariable->GetShaderDesc( passShaderDesc.ShaderIndex, &effectShaderDesc );

        D3D11_SIGNATURE_PARAMETER_DESC signatureParameterDesc{};
        std::vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
        uint32_t stride = 0;

        for ( uint32_t i{}; i < effectShaderDesc.NumInputSignatureEntries; ++i )
        {
            passShaderDesc.pShaderVariable->GetInputSignatureElementDesc(
                passShaderDesc.ShaderIndex, i, &signatureParameterDesc );

            uint32_t offset = static_cast< uint32_t >( floor( log( signatureParameterDesc.Mask ) / log( 2 ) ) + 1 ) * 4;
            DXGI_FORMAT type{};

            switch ( signatureParameterDesc.ComponentType )
            {
                case D3D10_REGISTER_COMPONENT_FLOAT32:
                    if ( signatureParameterDesc.Mask == 1 )
                        type = DXGI_FORMAT_R32_FLOAT;
                    else if ( signatureParameterDesc.Mask == 3 )
                        type = DXGI_FORMAT_R32G32_FLOAT;
                    else if ( signatureParameterDesc.Mask == 7 )
                        type = DXGI_FORMAT_R32G32B32_FLOAT;
                    else
                        type = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    break;
                case D3D10_REGISTER_COMPONENT_UINT32:
                    if ( signatureParameterDesc.Mask == 1 )
                        type = DXGI_FORMAT_R32_UINT;
                    else if ( signatureParameterDesc.Mask == 3 )
                        type = DXGI_FORMAT_R32G32_UINT;
                    else if ( signatureParameterDesc.Mask == 7 )
                        type = DXGI_FORMAT_R32G32B32_UINT;
                    else
                        type = DXGI_FORMAT_R32G32B32A32_UINT;
                    break;
                case D3D10_REGISTER_COMPONENT_SINT32:
                    if ( signatureParameterDesc.Mask == 1 )
                        type = DXGI_FORMAT_R32_SINT;
                    else if ( signatureParameterDesc.Mask == 3 )
                        type = DXGI_FORMAT_R32G32_SINT;
                    else if ( signatureParameterDesc.Mask == 7 )
                        type = DXGI_FORMAT_R32G32B32_SINT;
                    else
                        type = DXGI_FORMAT_R32G32B32A32_SINT;
                    break;
                default:
                    SM_LOG_ERROR( "DirectX11Shader::BuildInputLayout() > Unsupported Component Type" );
                    break;
            }

            D3D11_INPUT_ELEMENT_DESC inputLayout = { signatureParameterDesc.SemanticName,
                signatureParameterDesc.SemanticIndex,
                type,
                0,
                stride,
                D3D11_INPUT_PER_VERTEX_DATA,
                0 };

            inputDescs.push_back( inputLayout );

            BufferElement element{ DirectXBaseTypeToShaderDataType( type ), signatureParameterDesc.SemanticName };
            m_BufferLayout.AddElement( element );

            stride += offset;
        }

        uint32_t count{ static_cast< uint32_t >( inputDescs.size() ) };

        D3DX11_PASS_DESC passDesc{};
        m_pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
        HRESULT result = m_pDirectX11Context->GetDevice()->CreateInputLayout(
            inputDescs.data(), count, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout );

        if ( FAILED( result ) )
            SM_LOG_ERROR( "DirectX11Shader::BuildInputLayout > Failed to create input layout: %ls",
                GetDirectX11ErrorMessage( result ) );
    }

    ID3DX11EffectVariable *DirectX11Shader::GetEffectVariable( const std::string &sementicName )
    {
        if ( m_EffectVariableMap.find( sementicName ) != m_EffectVariableMap.end() )
            return m_EffectVariableMap[sementicName];

        auto pEffectVariable = m_pEffect->GetVariableBySemantic( sementicName.c_str() );
        if ( !pEffectVariable->IsValid() )
        {
            SM_LOG_WARNING( "DirectX11Shader::GetEffectVariable > Invalid effect variable: %s", sementicName );
            return nullptr;
        }

        m_EffectVariableMap[sementicName] = pEffectVariable;
        return pEffectVariable;
    }
}