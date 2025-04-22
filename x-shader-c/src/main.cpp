#include <Windows.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

constexpr uint32_t SHADER_FILE_MAGIC = 0x53484452; // 'SHDR'
constexpr uint32_t SHADER_FILE_VERSION = 1;

std::string GetD3D11ShaderVariableType( D3D11_SHADER_TYPE_DESC typeDesc )
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

std::string GetD3D11ParamType( D3D11_SIGNATURE_PARAMETER_DESC paramDesc )
{
    switch ( paramDesc.ComponentType )
    {
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            if ( paramDesc.Mask == 1 )
                return "Float";
            else if ( paramDesc.Mask == 3 )
                return "Float2";
            else if ( paramDesc.Mask == 7 )
                return "Float3";
            else if ( paramDesc.Mask == 15 )
                return "Float4";
            else
                return "Unknown";
        case D3D10_REGISTER_COMPONENT_UINT32:
            if ( paramDesc.Mask == 1 )
                return "UInt";
            else if ( paramDesc.Mask == 3 )
                return "UInt2";
            else if ( paramDesc.Mask == 7 )
                return "Uint3";
            else if ( paramDesc.Mask == 15 )
                return "Uint4";
            else
                return "Unknown";
        case D3D10_REGISTER_COMPONENT_SINT32:
            if ( paramDesc.Mask == 1 )
                return "Int";
            else if ( paramDesc.Mask == 3 )
                return "Int2";
            else if ( paramDesc.Mask == 7 )
                return "Int3";
            else if ( paramDesc.Mask == 15 )
                return "Int4";
            else
                return "Unknown";
        default:
            return "Unknown";
    }
}

std::string GetD3D11ShaderInputType( D3D_SHADER_INPUT_TYPE inputType )
{
    switch ( inputType )
    {
        case D3D_SIT_CBUFFER:
            return "ConstantBuffer";
        case D3D_SIT_TEXTURE:
            return "Texture";
        case D3D_SIT_SAMPLER:
            return "Sampler";
        default:
            return "Unknown";
    }
}

Microsoft::WRL::ComPtr< ID3DBlob >
CompileShader( const std::filesystem::path &inputFile, const std::string &entryPoint, const std::string &targetProfile )
{
    std::ifstream input{ inputFile };
    if ( !input.is_open() )
    {
        std::cerr << "Failed to open shader file: " << inputFile << '\n';
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
        entryPoint.c_str(),
        targetProfile.c_str(),
        D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
        0,
        &pCompiledBlob,
        &pErrorBlob );

    if ( FAILED( hr ) )
    {
        if ( pErrorBlob )
        {
            std::cerr << "Shader compile error: " << reinterpret_cast< char * >( pErrorBlob->GetBufferPointer() )
                      << '\n';
        }
        else
        {
            std::cerr << "Unknown shader compile error.\n";
        }

        return nullptr;
    }

    return pCompiledBlob;
}

YAML::Node ReflectBlob( Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob,
    const std::string &entryPoint,
    const std::string &targetProfile )
{
    YAML::Node yaml;

    Microsoft::WRL::ComPtr< ID3D11ShaderReflection > pReflector;
    if ( FAILED( D3DReflect(
             pCompiledBlob->GetBufferPointer(), pCompiledBlob->GetBufferSize(), IID_PPV_ARGS( &pReflector ) ) ) )
    {
        std::cerr << "Failed to reflect shader\n";
        return yaml;
    }

    D3D11_SHADER_DESC shaderDesc;
    pReflector->GetDesc( &shaderDesc );

    yaml["EntryPoint"] = entryPoint;
    yaml["TargetProfile"] = targetProfile;
    yaml["BlobFormat"] = "dxbc";

    YAML::Node inputs;
    for ( UINT i = 0; i < shaderDesc.InputParameters; ++i )
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        if ( SUCCEEDED( pReflector->GetInputParameterDesc( i, &paramDesc ) ) )
        {
            YAML::Node node;
            node["SemanticName"] = paramDesc.SemanticName;
            node["SemanticIndex"] = paramDesc.SemanticIndex;
            node["Type"] = GetD3D11ParamType( paramDesc );
            node["Register"] = paramDesc.Register;
            inputs.push_back( node );
        }
    }
    yaml["InputSignature"] = inputs;

    YAML::Node outputs;
    for ( UINT i = 0; i < shaderDesc.OutputParameters; ++i )
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        if ( SUCCEEDED( pReflector->GetOutputParameterDesc( i, &paramDesc ) ) )
        {
            YAML::Node node;
            node["SemanticName"] = paramDesc.SemanticName;
            node["SemanticIndex"] = paramDesc.SemanticIndex;
            node["Type"] = GetD3D11ParamType( paramDesc );
            node["Register"] = paramDesc.Register;
            outputs.push_back( node );
        }
    }
    yaml["OutputSignature"] = outputs;

    YAML::Node cbuffers;
    for ( UINT i = 0; i < shaderDesc.ConstantBuffers; ++i )
    {
        auto cb = pReflector->GetConstantBufferByIndex( i );
        D3D11_SHADER_BUFFER_DESC cbDesc;
        if ( SUCCEEDED( cb->GetDesc( &cbDesc ) ) )
        {
            YAML::Node cbNode;
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

    YAML::Node resources;
    for ( UINT i = 0; i < shaderDesc.BoundResources; ++i )
    {
        D3D11_SHADER_INPUT_BIND_DESC resDesc;
        pReflector->GetResourceBindingDesc( i, &resDesc );

        YAML::Node resNode;
        resNode["Name"] = resDesc.Name;
        resNode["Type"] = GetD3D11ShaderInputType( resDesc.Type );
        resNode["BindPoint"] = resDesc.BindPoint;
        resNode["BindCount"] = resDesc.BindCount;
        resources.push_back( resNode );
    }
    yaml["Resources"] = resources;

    return yaml;

    /*std::stringstream yamlStream;
    yamlStream << yaml;
    std::string yamlSerialized = yamlStream.str();

    std::ofstream outFile( outputShaderFile, std::ios::binary );
    if ( !outFile )
    {
        std::cerr << "Failed to open output file: " << outputShaderFile << std::endl;
        return;
    }

    uint32_t blobSize = static_cast< uint32_t >( compiledBlob->GetBufferSize() );
    uint32_t yamlSize = static_cast< uint32_t >( yamlSerialized.size() );

    outFile.write( reinterpret_cast< const char * >( &SHADER_FILE_MAGIC ), sizeof( uint32_t ) );
    outFile.write( reinterpret_cast< const char * >( &SHADER_FILE_VERSION ), sizeof( uint32_t ) );
    outFile.write( reinterpret_cast< const char * >( &blobSize ), sizeof( uint32_t ) );
    outFile.write( reinterpret_cast< const char * >( &yamlSize ), sizeof( uint32_t ) );
    outFile.write( reinterpret_cast< const char * >( compiledBlob->GetBufferPointer() ), blobSize );
    outFile.write( yamlSerialized.data(), yamlSize );

    std::cout << "Shader compiled and saved to compound .shader format: " << outputShaderFile << std::endl;*/
}

int main( int argc, char *argv[] )
{
    std::filesystem::path inputFile{ "pbr.hlsl" };
    std::filesystem::path outputFile{ "pbr.shader" };
    std::string entryPoint{ "PSMain" };
    std::string targetProfile{ "ps_5_0" };

    Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob = CompileShader( inputFile, entryPoint, targetProfile );
    if ( !pCompiledBlob )
        return 2;

    YAML::Node data = ReflectBlob( pCompiledBlob, entryPoint, targetProfile );
    std::ofstream outputStream{ outputFile };
    outputStream << data;

    return 0;
}