#include <Windows.h>
#include <dxcapi.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdint.h>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

constexpr uint32_t SHADER_FILE_MAGIC = 0x53484452; // 'SHDR'
constexpr uint32_t SHADER_FILE_VERSION = 1;

Microsoft::WRL::ComPtr< IDxcBlob > CompileShader( const std::filesystem::path &inputFile,
    const std::wstring &entryPoint,
    const std::wstring &targetProfile )
{
    Microsoft::WRL::ComPtr< IDxcUtils > pUtils;
    if ( FAILED( DxcCreateInstance( CLSID_DxcUtils, IID_PPV_ARGS( &pUtils ) ) ) )
    {
        std::cerr << "Failed to create DxcUtils instance\n";
        return nullptr;
    }

    Microsoft::WRL::ComPtr< IDxcCompiler3 > pCompiler;
    if ( FAILED( DxcCreateInstance( CLSID_DxcCompiler, IID_PPV_ARGS( &pCompiler ) ) ) )
    {
        std::cerr << "Failed to create DxcCompiler instance\n";
        return nullptr;
    }

    Microsoft::WRL::ComPtr< IDxcIncludeHandler > pIncludeHandler;
    pUtils->CreateDefaultIncludeHandler( &pIncludeHandler );

    Microsoft::WRL::ComPtr< IDxcBlobEncoding > sourceBlob;
    if ( FAILED( pUtils->LoadFile( inputFile.c_str(), nullptr, &sourceBlob ) ) )
    {
        std::cerr << "Failed to load shader file: " << inputFile << '\n';
        return nullptr;
    }

    DxcBuffer sourceBuffer{ sourceBlob->GetBufferPointer(), sourceBlob->GetBufferSize(), DXC_CP_UTF8 };

    // Compile with debug info for full reflection
    std::vector< LPCWSTR > compileArgs{
        inputFile.c_str(), L"-E", entryPoint.c_str(), L"-T", targetProfile.c_str(), L"-Zi", L"-Qembed_debug" };

    Microsoft::WRL::ComPtr< IDxcResult > pResult;
    if ( FAILED( pCompiler->Compile( &sourceBuffer,
             compileArgs.data(),
             static_cast< UINT >( compileArgs.size() ),
             pIncludeHandler.Get(),
             IID_PPV_ARGS( &pResult ) ) ) )
    {
        std::cerr << "Failed to compile shader\n";
        return nullptr;
    }

    Microsoft::WRL::ComPtr< IDxcBlob > pCompiledBlob;
    pResult->GetOutput( DXC_OUT_OBJECT, IID_PPV_ARGS( &pCompiledBlob ), nullptr );

    Microsoft::WRL::ComPtr< IDxcBlobUtf8 > pErrors;
    pResult->GetOutput( DXC_OUT_ERRORS, IID_PPV_ARGS( &pErrors ), nullptr );
    if ( pErrors && pErrors->GetStringLength() > 0 )
    {
        std::cerr << "Shader compile errors:\n" << pErrors->GetStringPointer() << '\n';
    }

    return pCompiledBlob;
}

YAML::Node ReflectBlob( Microsoft::WRL::ComPtr< IDxcBlob > pCompiledBlob,
    const std::wstring &entryPoint,
    const std::wstring &targetProfile )
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

    yaml["EntryPoint"] = std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > >().to_bytes( entryPoint );
    yaml["TargetProfile"] = std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > >().to_bytes( targetProfile );
    yaml["BlobFormat"] = "dxil";

    YAML::Node cbuffers;
    for ( UINT i = 0; i < shaderDesc.ConstantBuffers; ++i )
    {
        auto cb = pReflector->GetConstantBufferByIndex( i );
        D3D11_SHADER_BUFFER_DESC cbDesc;
        cb->GetDesc( &cbDesc );

        YAML::Node cbNode;
        cbNode["Name"] = cbDesc.Name;
        cbNode["Size"] = cbDesc.Size;
        cbNode["Variables"] = YAML::Node( YAML::NodeType::Sequence );

        for ( UINT j = 0; j < cbDesc.Variables; ++j )
        {
            auto var = cb->GetVariableByIndex( j );
            D3D11_SHADER_VARIABLE_DESC varDesc;
            var->GetDesc( &varDesc );

            YAML::Node varNode;
            varNode["Name"] = varDesc.Name;
            varNode["Size"] = varDesc.Size;
            varNode["Offset"] = varDesc.StartOffset;
            cbNode["Variables"].push_back( varNode );
        }

        cbuffers.push_back( cbNode );
    }
    yaml["ConstantBuffers"] = cbuffers;

    YAML::Node resources;
    for ( UINT i = 0; i < shaderDesc.BoundResources; ++i )
    {
        D3D11_SHADER_INPUT_BIND_DESC resDesc;
        pReflector->GetResourceBindingDesc( i, &resDesc );

        YAML::Node resNode;
        resNode["Name"] = resDesc.Name;
        resNode["Type"] = static_cast< uint32_t >( resDesc.Type ); // TODO: convert type
        resNode["BindPoint"] = resDesc.BindPoint;
        resNode["BindCount"] = resDesc.BindCount;
        resources.push_back( resNode );
    }
    yaml["resources"] = resources;

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
    std::wstring entryPoint{ L"PSMain" };
    std::wstring targetProfile{ L"ps_6_6" };

    Microsoft::WRL::ComPtr< IDxcBlob > pCompiledBlob = CompileShader( inputFile, entryPoint, targetProfile );
    if ( !pCompiledBlob )
        return 2;

    YAML::Node data = ReflectBlob( pCompiledBlob, entryPoint, targetProfile );

    return 0;
}