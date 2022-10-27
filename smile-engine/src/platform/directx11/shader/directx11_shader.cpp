#include "smpch.h"
#include "directx11_shader.h"

#include "smile_engine/core/application.h"
#include "platform/directX11/directx11_diagnostics.h"

#include <d3dcompiler.h>

namespace smile::graphic
{
    static DXGI_FORMAT shaderDataTypeToDirectXBaseType( ShaderDataType type )
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
                SM_ASSERT( false, "DirectX11Shader::shaderDataTypeToDirectXBaseType > Unknown ShaderDataType" );
                return DXGI_FORMAT_UNKNOWN;
        }
    }

    static ShaderDataType directXBaseTypeToShaderDataType( DXGI_FORMAT type )
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
                SM_ASSERT( false, "DirectX11Shader::directXBaseTypeToShaderDataType > Unknown DXGI Type" );
                return ShaderDataType::None;
        }
    }

    DirectX11Shader::DirectX11Shader( const std::string &asset_file,
        const BufferLayout &layout,
        const std::string &technique_name )
        : bufferLayout{ layout }
    {
        initalize( asset_file, technique_name );
        buildInputLayout( layout );
    }

    DirectX11Shader::DirectX11Shader( const std::string &asset_file, const std::string &technique_name )
    {
        initalize( asset_file, technique_name );
        buildInputLayout();
    }

    void DirectX11Shader::initalize( const std::string &asset_file, const std::string &technique_name )
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( directX11Context, "DirectX11Shader > Rendering context is not a DirectX 11 Graphics Context" );

        if ( !loadEffect( directX11Context->getDevice(), asset_file ) )
        {
            SAFE_RELEASE( effect );
            SM_ASSERT( false, "DirectX11Shader > Failed to load effect" );
        }

        if ( !technique_name.empty() )
            technique = effect->GetTechniqueByName( technique_name.c_str() );
        else
            technique = effect->GetTechniqueByIndex( 0 );

        if ( !technique->IsValid() )
            SM_LOG_WARNING( "DirectX11Shader > Invalid technique" );

        setName( asset_file );
    }

    void DirectX11Shader::setName( const std::string &asset_file )
    {
        // Find name from asset path
        auto last_slash = asset_file.find_last_of( "/\\" );
        last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
        auto last_dot = asset_file.rfind( '.' );
        auto count = last_dot == std::string::npos ? asset_file.size() - last_slash : last_dot - last_slash;
        name = asset_file.substr( last_slash, count );
    }

    DirectX11Shader::~DirectX11Shader()
    {
        for ( auto &effect_var : effectVariableMap )
        {
            SAFE_RELEASE( effect_var.second );
        }

        effectVariableMap.clear();

        SAFE_RELEASE( inputLayout );
        SAFE_RELEASE( technique );
        SAFE_RELEASE( effect );
    }

    void DirectX11Shader::bind() const
    {
        directX11Context->getDeviceContext()->IASetInputLayout( inputLayout );
    }

    void DirectX11Shader::unbind() const
    {
        directX11Context->getDeviceContext()->IASetInputLayout( nullptr );
    }

    void DirectX11Shader::uploadMat4( const std::string &sementic_name, const DirectX::XMFLOAT4X4 &matrix )
    {
        auto matrixVariable = getEffectVariable( sementic_name )->AsMatrix();
        if ( matrixVariable->IsValid() )
        {
            matrixVariable->SetMatrix( &matrix._11 );
        }
    }

    void DirectX11Shader::uploadMat4Array( const std::string &sementic_name,
        const std::vector< DirectX::XMFLOAT4X4 > &mat_array )
    {
        auto matArrayVariable = getEffectVariable( sementic_name )->AsMatrix();
        if ( matArrayVariable->IsValid() )
        {
            matArrayVariable->SetMatrixArray( &mat_array[0]._11, 0, static_cast< Uint32 >( mat_array.size() ) );
        }
    }

    void DirectX11Shader::uploadFloat2( const std::string &sementic_name, const DirectX::XMFLOAT2 &value )
    {
        auto vectorVariable = getEffectVariable( sementic_name )->AsVector();
        if ( vectorVariable->IsValid() )
        {
            vectorVariable->SetFloatVector( &value.x );
        }
    }

    void DirectX11Shader::uploadFloat3( const std::string &sementic_name, const DirectX::XMFLOAT3 &value )
    {
        auto vectorVariable = getEffectVariable( sementic_name )->AsVector();
        if ( vectorVariable->IsValid() )
        {
            vectorVariable->SetFloatVector( &value.x );
        }
    }

    void DirectX11Shader::uploadInt( const std::string &sementic_name, int value )
    {
        auto int_variable = getEffectVariable( sementic_name )->AsScalar();
        if ( int_variable->IsValid() )
        {
            int_variable->SetInt( value );
        }
    }

    void DirectX11Shader::uploadTexture2D( const std::string &sementic_name, const Ref< Texture2D > &texture_2d )
    {
        auto texture_variable = getEffectVariable( sementic_name )->AsShaderResource();
        if ( texture_variable->IsValid() )
        {
            texture_variable->SetResource( ( ID3D11ShaderResourceView * )texture_2d->getData() );
        }
    }

    void DirectX11Shader::uploadBool( const std::string &sementic_name, bool value )
    {
        auto bool_variable = getEffectVariable( sementic_name )->AsScalar();
        if ( bool_variable->IsValid() )
        {
            bool_variable->SetBool( value );
        }
    }

    void DirectX11Shader::uploadFloat( const std::string &sementic_name, float value )
    {
        auto float_variable = getEffectVariable( sementic_name )->AsScalar();
        if ( float_variable->IsValid() )
        {
            float_variable->SetFloat( value );
        }
    }

    bool DirectX11Shader::loadEffect( ID3D11Device *device, const std::string &asset_file )
    {
        HRESULT result{ S_OK };
        ID3D10Blob *error_blob{ nullptr };

        DWORD shader_flags{ 0 };
#if defined( DEBUG ) || defined( _DEBUG )
        shader_flags |= D3DCOMPILE_DEBUG;
        shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        std::wstring asset_file_wide_str{ asset_file.begin(), asset_file.end() };
        result = D3DX11CompileEffectFromFile(
            asset_file_wide_str.c_str(), nullptr, nullptr, shader_flags, 0, device, &effect, &error_blob );

        if ( FAILED( result ) )
        {
            if ( error_blob )
            {
                char *errors{ ( char * )error_blob->GetBufferPointer() };

                std::wstringstream ss;
                for ( unsigned int i{}; i < error_blob->GetBufferSize(); ++i )
                {
                    ss << errors[i];
                }

                OutputDebugStringW( ss.str().c_str() );
                error_blob->Release();
                error_blob = nullptr;

                SM_LOG_ERROR( "%s", ss.str() );
            }
            else
            {
                SM_LOG_ERROR( "DirectX11Shader::loadEffect > Failed to CreateEffectFromFile: %s , error: %ls",
                    asset_file.c_str(),
                    getDirectX11ErrorMessage( result ) );
            }

            return false;
        }

        return true;
    }

    void DirectX11Shader::buildInputLayout( const BufferLayout &layout )
    {
        std::vector< D3D11_INPUT_ELEMENT_DESC > input_descs{};
        for ( const auto &element : layout )
        {
            input_descs.push_back( D3D11_INPUT_ELEMENT_DESC{ element.name.c_str(),
                0,
                shaderDataTypeToDirectXBaseType( element.dataType ),
                0,
                element.offset,
                D3D11_INPUT_PER_VERTEX_DATA,
                0 } );
        }

        Uint32 count{ static_cast< Uint32 >( input_descs.size() ) };

        D3DX11_PASS_DESC pass_desc{};
        technique->GetPassByIndex( 0 )->GetDesc( &pass_desc );
        HRESULT result = directX11Context->getDevice()->CreateInputLayout(
            input_descs.data(), count, pass_desc.pIAInputSignature, pass_desc.IAInputSignatureSize, &inputLayout );

        if ( FAILED( result ) )
            SM_LOG_ERROR( "DirectX11Shader::buildInputLayout > Failed to create input layout: %ls",
                getDirectX11ErrorMessage( result ) );
    }

    void DirectX11Shader::buildInputLayout()
    {
        D3DX11_PASS_SHADER_DESC pass_shader_desc{};
        technique->GetPassByIndex( 0 )->GetVertexShaderDesc( &pass_shader_desc );

        D3DX11_EFFECT_SHADER_DESC effectShaderDesc{};
        pass_shader_desc.pShaderVariable->GetShaderDesc( pass_shader_desc.ShaderIndex, &effectShaderDesc );

        D3D11_SIGNATURE_PARAMETER_DESC signature_parameter_desc{};
        std::vector< D3D11_INPUT_ELEMENT_DESC > input_descs{};
        Uint32 stride = 0;

        for ( Uint32 i{}; i < effectShaderDesc.NumInputSignatureEntries; ++i )
        {
            pass_shader_desc.pShaderVariable->GetInputSignatureElementDesc(
                pass_shader_desc.ShaderIndex, i, &signature_parameter_desc );

            Uint32 offset = static_cast< Uint32 >( floor( log( signature_parameter_desc.Mask ) / log( 2 ) ) + 1 ) * 4;
            DXGI_FORMAT type{};

            switch ( signature_parameter_desc.ComponentType )
            {
                case D3D10_REGISTER_COMPONENT_FLOAT32:
                    if ( signature_parameter_desc.Mask == 1 )
                        type = DXGI_FORMAT_R32_FLOAT;
                    else if ( signature_parameter_desc.Mask == 3 )
                        type = DXGI_FORMAT_R32G32_FLOAT;
                    else if ( signature_parameter_desc.Mask == 7 )
                        type = DXGI_FORMAT_R32G32B32_FLOAT;
                    else
                        type = DXGI_FORMAT_R32G32B32A32_FLOAT;
                    break;
                case D3D10_REGISTER_COMPONENT_UINT32:
                    if ( signature_parameter_desc.Mask == 1 )
                        type = DXGI_FORMAT_R32_UINT;
                    else if ( signature_parameter_desc.Mask == 3 )
                        type = DXGI_FORMAT_R32G32_UINT;
                    else if ( signature_parameter_desc.Mask == 7 )
                        type = DXGI_FORMAT_R32G32B32_UINT;
                    else
                        type = DXGI_FORMAT_R32G32B32A32_UINT;
                    break;
                case D3D10_REGISTER_COMPONENT_SINT32:
                    if ( signature_parameter_desc.Mask == 1 )
                        type = DXGI_FORMAT_R32_SINT;
                    else if ( signature_parameter_desc.Mask == 3 )
                        type = DXGI_FORMAT_R32G32_SINT;
                    else if ( signature_parameter_desc.Mask == 7 )
                        type = DXGI_FORMAT_R32G32B32_SINT;
                    else
                        type = DXGI_FORMAT_R32G32B32A32_SINT;
                    break;
                default:
                    SM_LOG_ERROR( "DirectX11Shader::buildInputLayout() > Unsupported Component Type" );
                    break;
            }

            D3D11_INPUT_ELEMENT_DESC input_layout = { signature_parameter_desc.SemanticName,
                signature_parameter_desc.SemanticIndex,
                type,
                0,
                stride,
                D3D11_INPUT_PER_VERTEX_DATA,
                0 };

            input_descs.push_back( input_layout );

            BufferElement element{ directXBaseTypeToShaderDataType( type ), signature_parameter_desc.SemanticName };
            bufferLayout.addElement( element );

            stride += offset;
        }

        Uint32 count{ static_cast< Uint32 >( input_descs.size() ) };

        D3DX11_PASS_DESC pass_desc{};
        technique->GetPassByIndex( 0 )->GetDesc( &pass_desc );
        HRESULT result = directX11Context->getDevice()->CreateInputLayout(
            input_descs.data(), count, pass_desc.pIAInputSignature, pass_desc.IAInputSignatureSize, &inputLayout );

        if ( FAILED( result ) )
            SM_LOG_ERROR( "DirectX11Shader::buildInputLayout > Failed to create input layout: %ls",
                getDirectX11ErrorMessage( result ) );
    }

    ID3DX11EffectVariable *DirectX11Shader::getEffectVariable( const std::string &sementic_name )
    {
        if ( effectVariableMap.find( sementic_name ) != effectVariableMap.end() )
            return effectVariableMap[sementic_name];

        auto effect_variable = effect->GetVariableBySemantic( sementic_name.c_str() );
        if ( !effect_variable->IsValid() )
        {
            SM_LOG_WARNING( "DirectX11Shader::getEffectVariable > Invalid effect variable: %s", sementic_name );
            return nullptr;
        }

        effectVariableMap[sementic_name] = effect_variable;
        return effect_variable;
    }
}