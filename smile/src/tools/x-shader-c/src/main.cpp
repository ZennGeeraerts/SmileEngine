#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/std_string.h"
#include "smile/common/logging/logger.h"
#include "smile/core/yaml/string.h"

#include <Windows.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include <fstream>
#include <iostream>
#include <filesystem>

namespace smile
{
    constexpr Uint32 g_ShaderFileMagic = 0x53484452; // 'SHDR'
    constexpr Uint32 g_ShaderFileVersion = 1;

    primitive::String GetD3D11ShaderVariableType( D3D11_SHADER_TYPE_DESC typeDesc )
    {
        switch ( typeDesc.Class )
        {
            case D3D_SVC_SCALAR:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_FLOAT:
                        return "Float";
                    case D3D_SVT_BOOL:
                        return "Bool";
                    case D3D_SVT_INT:
                        return "Int";
                    default:
                        return "Unknown";
                }
            }
            case D3D_SVC_VECTOR:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        switch ( typeDesc.Columns )
                        {
                            case 2:
                                return "Float2";
                            case 3:
                                return "Float3";
                            default:
                                return "Float3";
                        }
                    }
                    default:
                        return "Unknown";
                }
            }
            case D3D_SVC_MATRIX_COLUMNS:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        if ( ( typeDesc.Columns == 4 ) && ( typeDesc.Rows == 4 ) )
                        {
                            if ( typeDesc.Elements == 0 )
                                return "Mat4";
                            else
                                return "Mat4Array";
                        }

                        return "Unknown";
                    }
                    default:
                        return "Unknown";
                }
            }
            case D3D_SVC_OBJECT:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_TEXTURE:
                        return "Texture";
                    case D3D_SVT_TEXTURE2D:
                        return "Texture2D";
                    case D3D_SVT_TEXTURE3D:
                        return "Texture3D";
                    case D3D_SVT_TEXTURECUBE:
                        return "TextureCube";
                    default:
                        return "Unknown";
                }
            }
            default:
                return "Unknown";
        }
    }

    primitive::String GetD3D11ParamFormat( D3D11_SIGNATURE_PARAMETER_DESC paramDesc )
    {
        switch ( paramDesc.ComponentType )
        {
            case D3D10_REGISTER_COMPONENT_FLOAT32:
                if ( paramDesc.Mask == 1 )
                    return "R32_FLOAT";
                else if ( paramDesc.Mask == 3 )
                    return "RG32_FLOAT";
                else if ( paramDesc.Mask == 7 )
                    return "RGB32_FLOAT";
                else if ( paramDesc.Mask == 15 )
                    return "RGBA32_FLOAT";
                else
                    return "UNKNOWN";
            case D3D10_REGISTER_COMPONENT_UINT32:
                if ( paramDesc.Mask == 1 )
                    return "R32_UINT";
                else if ( paramDesc.Mask == 3 )
                    return "RG32_UINT";
                else if ( paramDesc.Mask == 7 )
                    return "RGB32_UINT";
                else if ( paramDesc.Mask == 15 )
                    return "RGBA32_UINT";
                else
                    return "UNKNOWN";
            case D3D10_REGISTER_COMPONENT_SINT32:
                if ( paramDesc.Mask == 1 )
                    return "R32_SINT";
                else if ( paramDesc.Mask == 3 )
                    return "RG32_SINT";
                else if ( paramDesc.Mask == 7 )
                    return "RGB32_SINT";
                else if ( paramDesc.Mask == 15 )
                    return "RGBA32_SINT";
                else
                    return "UNKNOWN";
            default:
                return "UNKNOWN";
        }
    }

    primitive::String GetD3D11ShaderInputType( const D3D11_SHADER_INPUT_BIND_DESC &resDesc )
    {
        switch ( resDesc.Type )
        {
            case D3D_SIT_CBUFFER:
                return "ConstantBuffer";

            case D3D_SIT_TBUFFER:
                return "TypedBuffer_SRV";

            case D3D_SIT_TEXTURE:
                return "Texture_SRV";

            case D3D_SIT_SAMPLER:
                return "Sampler";

            case D3D_SIT_UAV_RWTYPED:
                switch ( resDesc.Dimension )
                {
                    case D3D_SRV_DIMENSION_BUFFER:
                    case D3D_SRV_DIMENSION_BUFFEREX:
                        return "TypedBuffer_UAV";
                    case D3D_SRV_DIMENSION_TEXTURE1D:
                    case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
                    case D3D_SRV_DIMENSION_TEXTURE2D:
                    case D3D_SRV_DIMENSION_TEXTURE2DARRAY:
                    case D3D_SRV_DIMENSION_TEXTURE3D:
                        return "Texture_UAV";
                    default:
                        return "Unknown";
                }

            case D3D_SIT_UAV_RWSTRUCTURED:
            case D3D_SIT_UAV_APPEND_STRUCTURED:
            case D3D_SIT_UAV_CONSUME_STRUCTURED:
                if ( resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER || resDesc.Dimension == D3D_SRV_DIMENSION_BUFFEREX )
                    return "StructuredBuffer_UAV";
                else
                    return "Unknown";

            case D3D_SIT_UAV_RWBYTEADDRESS:
                if ( resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER || resDesc.Dimension == D3D_SRV_DIMENSION_BUFFEREX )
                    return "RawBuffer_UAV";
                else
                    return "Unknown";

            case D3D_SIT_STRUCTURED:
                return "StructuredBuffer_SRV";

            case D3D_SIT_BYTEADDRESS:
                return "RawBuffer_SRV";

            case D3D_SIT_RTACCELERATIONSTRUCTURE:
                return "RayTracingAccelStruct";

            default:
                return "Unknown";
        }
    }

    Microsoft::WRL::ComPtr< ID3DBlob > CompileShader( const std::filesystem::path &inputFile,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        std::ifstream input{ inputFile };
        if ( !input.is_open() )
        {
            SM_LOG_ERROR( "Failed to open shader file: {}", inputFile.string() );
            return nullptr;
        }

        std::string source{ ( std::istreambuf_iterator< char >{ input } ), std::istreambuf_iterator< char >{} };

        Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob;
        Microsoft::WRL::ComPtr< ID3DBlob > pErrorBlob;

        HRESULT hr = D3DCompile( source.c_str(),
            source.size(),
            inputFile.filename().string().c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.GetData(),
            targetProfile.GetData(),
            D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
            0,
            &pCompiledBlob,
            &pErrorBlob );

        if ( FAILED( hr ) )
        {
            if ( pErrorBlob )
            {
                SM_LOG_ERROR(
                    "Shader compile error: {}", reinterpret_cast< char * >( pErrorBlob->GetBufferPointer() ) );
            }
            else
            {
                SM_LOG_ERROR( "Unknown shader compile error." );
            }

            return nullptr;
        }

        return pCompiledBlob;
    }

    yaml::Node ReflectBlob( Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob,
        const primitive::String &entryPoint,
        const primitive::String &targetProfile )
    {
        yaml::Node yaml;

        Microsoft::WRL::ComPtr< ID3D11ShaderReflection > pReflector;
        if ( FAILED( D3DReflect(
                 pCompiledBlob->GetBufferPointer(), pCompiledBlob->GetBufferSize(), IID_PPV_ARGS( &pReflector ) ) ) )
        {
            SM_LOG_ERROR( "Failed to reflect shader" );
            return yaml;
        }

        D3D11_SHADER_DESC shaderDesc;
        pReflector->GetDesc( &shaderDesc );

        yaml["EntryPoint"] = entryPoint;
        yaml["TargetProfile"] = targetProfile;
        yaml["BlobFormat"] = "dxbc";

        yaml::Node inputs;
        for ( UINT i = 0; i < shaderDesc.InputParameters; ++i )
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            if ( FAILED( pReflector->GetInputParameterDesc( i, &paramDesc ) ) )
            {
                SM_LOG_ERROR( "Failed to reflect input parameters" );
                return yaml;
            }

            yaml::Node node;
            node["SemanticName"] = paramDesc.SemanticName;
            node["SemanticIndex"] = paramDesc.SemanticIndex;
            node["Format"] = GetD3D11ParamFormat( paramDesc );
            node["Register"] = paramDesc.Register;
            inputs.push_back( node );
        }

        yaml["InputSignature"] = inputs;

        yaml::Node outputs;
        for ( UINT i = 0; i < shaderDesc.OutputParameters; ++i )
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            if ( FAILED( pReflector->GetOutputParameterDesc( i, &paramDesc ) ) )
            {
                SM_LOG_ERROR( "Failed to reflect output parameters" );
                return yaml;
            }

            yaml::Node node;
            node["SemanticName"] = paramDesc.SemanticName;
            node["SemanticIndex"] = paramDesc.SemanticIndex;
            node["Format"] = GetD3D11ParamFormat( paramDesc );
            node["Register"] = paramDesc.Register;
            outputs.push_back( node );
        }

        yaml["OutputSignature"] = outputs;

        yaml::Node cbuffers;
        for ( UINT i = 0; i < shaderDesc.ConstantBuffers; ++i )
        {
            auto cb = pReflector->GetConstantBufferByIndex( i );
            D3D11_SHADER_BUFFER_DESC cbDesc;
            if ( SUCCEEDED( cb->GetDesc( &cbDesc ) ) )
            {
                yaml::Node cbNode;
                cbNode["Name"] = cbDesc.Name;
                cbNode["Size"] = cbDesc.Size;
                cbNode["Variables"] = YAML::Node{ YAML::NodeType::Sequence };

                for ( UINT j = 0; j < cbDesc.Variables; ++j )
                {
                    auto pVar = cb->GetVariableByIndex( j );

                    D3D11_SHADER_VARIABLE_DESC varDesc;
                    if ( SUCCEEDED( pVar->GetDesc( &varDesc ) ) )
                    {
                        ID3D11ShaderReflectionType *pType = pVar->GetType();
                        D3D11_SHADER_TYPE_DESC typeDesc;
                        pType->GetDesc( &typeDesc );

                        YAML::Node varNode;
                        varNode["Name"] = varDesc.Name;
                        varNode["Type"] = GetD3D11ShaderVariableType( typeDesc );
                        varNode["Size"] = varDesc.Size;
                        varNode["Offset"] = varDesc.StartOffset;
                        cbNode["Variables"].push_back( varNode );
                    }
                }

                cbuffers.push_back( cbNode );
            }
        }

        yaml["ConstantBuffers"] = cbuffers;

        yaml::Node resources;
        for ( UINT i = 0; i < shaderDesc.BoundResources; ++i )
        {
            D3D11_SHADER_INPUT_BIND_DESC resDesc;
            pReflector->GetResourceBindingDesc( i, &resDesc );

            YAML::Node resNode;
            resNode["Name"] = resDesc.Name;
            resNode["Type"] = GetD3D11ShaderInputType( resDesc );
            resNode["BindPoint"] = resDesc.BindPoint;
            resNode["BindCount"] = resDesc.BindCount;
            resources.push_back( resNode );
        }

        yaml["Resources"] = resources;

        return yaml;
    }

    bool WriteFile( Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob,
        const yaml::Node &yaml,
        const std::filesystem::path &outputFile )
    {
        yaml::Emitter yamlOutput{};
        yamlOutput << yaml;

        std::ofstream outStream{ outputFile, std::ios::binary };
        if ( !outStream )
        {
            SM_LOG_ERROR( "Failed to open output file: {}", outputFile.string() );
            return false;
        }

        uint32_t blobOffset = sizeof( uint32_t ) * 6;
        uint32_t blobSize = static_cast< uint32_t >( pCompiledBlob->GetBufferSize() );
        uint32_t yamlOffset = blobOffset + blobSize;
        uint32_t yamlSize = static_cast< uint32_t >( yamlOutput.size() );

        // Header
        outStream.write( reinterpret_cast< const char * >( &g_ShaderFileMagic ), sizeof( uint32_t ) );
        outStream.write( reinterpret_cast< const char * >( &g_ShaderFileVersion ), sizeof( uint32_t ) );
        outStream.write( reinterpret_cast< const char * >( &blobOffset ), sizeof( uint32_t ) );
        outStream.write( reinterpret_cast< const char * >( &blobSize ), sizeof( uint32_t ) );
        outStream.write( reinterpret_cast< const char * >( &yamlOffset ), sizeof( uint32_t ) );
        outStream.write( reinterpret_cast< const char * >( &yamlSize ), sizeof( uint32_t ) );

        // Blob
        outStream.write( reinterpret_cast< const char * >( pCompiledBlob->GetBufferPointer() ), blobSize );

        // YAML
        outStream.write( yamlOutput.c_str(), yamlSize );

        SM_LOG_INFO( "Shader compiled and saved to compound .smshader format: {}", outputFile.string() );
    }

    int main( int argc, char *argv[] )
    {
        if ( argc < 4 )
        {
            SM_LOG_ERROR( "Usage: x-shader-c <input_file> -o <output_file>" );
            return 1;
        }

        std::filesystem::path inputFile;
        std::filesystem::path outputFile;

        inputFile = argv[1];
        if ( primitive::StringView{ argv[2] } != "-o" )
        {
            SM_LOG_ERROR( "Expected '-o' after input file" );
            return 1;
        }
        outputFile = argv[3];

        primitive::String entryPoint;
        primitive::String targetProfile;

        primitive::StdString fileName = inputFile.filename().string();

        if ( fileName.Find( ".vs." ) != s_InvalidIndex )
        {
            entryPoint = "VSMain";
            targetProfile = "vs_5_0";
        }
        else if ( fileName.Find( ".ps." ) != s_InvalidIndex )
        {
            entryPoint = "PSMain";
            targetProfile = "ps_5_0";
        }
        else
        {
            SM_LOG_ERROR( "Could not detect whether a vertex shader or pixel shader was used" );
            return 1;
        }

        Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob = CompileShader( inputFile, entryPoint, targetProfile );

        if ( !pCompiledBlob )
            return 2;

        yaml::Node data = ReflectBlob( pCompiledBlob, entryPoint, targetProfile );

        if ( !WriteFile( pCompiledBlob, data, outputFile ) )
            return 3;

        return 0;
    }
}