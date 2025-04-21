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

    return pCompiledBlob;
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

    return 0;
}