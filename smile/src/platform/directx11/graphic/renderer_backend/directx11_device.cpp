/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_device.h"
#include "directx11_context.h"

#include "directx11_diagnostics.h"
#include "directx11_swap_chain.h"

#include "smile/core/window/window.h"

#include "resource/directx11_buffer.h"
#include "resource/directx11_texture.h"
#include "resource/directx11_frame_buffer.h"
#include "resource/directx11_rasterizer_state.h"

#include "shader/directx11_shader.h"

#include <d3dcompiler.h>

namespace smile::graphic
{
    namespace shaderhelpers
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

        static bool
        LoadEffect( ID3D11Device *pDevice, const memory::Ref< DirectX11Shader > &pShader, const std::string &assetFile )
        {
            HRESULT result{ S_OK };
            ID3D10Blob *pErrorBlob{ nullptr };

            DWORD shaderFlags{ 0 };
#ifdef SM_C_DEBUG
            shaderFlags |= D3DCOMPILE_DEBUG;
            shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
            std::wstring assetFileWideStr{ assetFile.begin(), assetFile.end() };
            result = D3DX11CompileEffectFromFile(
                assetFileWideStr.c_str(), nullptr, nullptr, shaderFlags, 0, pDevice, &pShader->pEffect, &pErrorBlob );

            if ( FAILED( result ) )
            {
                if ( pErrorBlob )
                {
                    LPVOID longPtrError = pErrorBlob->GetBufferPointer();
                    char *errors{ ( char * )longPtrError };

                    std::wstringstream ss;
                    for ( Uint32 i{}; i < pErrorBlob->GetBufferSize(); ++i )
                    {
                        ss << errors[i];
                    }

                    OutputDebugString( ss.str().c_str() );
                    pErrorBlob->Release();
                    pErrorBlob = nullptr;
                }
                else
                {
                    SM_LOG_ERROR( "DirectX11Device::LoadEffect > Failed to CreateEffectFromFile: {0} , error: {1}",
                        assetFile,
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                }

                return false;
            }

            return true;
        }

        static void BuildInputLayout( ID3D11Device *pDevice,
            const memory::Ref< DirectX11Shader > &pShader,
            const VertexLayout &layout )
        {
            std::vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
            for ( const auto &element : layout )
            {
                inputDescs.push_back( D3D11_INPUT_ELEMENT_DESC{ element.Name.c_str(),
                    0,
                    ShaderDataTypeToDirectXBaseType( element.DataType ),
                    0,
                    element.Offset,
                    D3D11_INPUT_PER_VERTEX_DATA,
                    0 } );
            }

            Uint32 count{ static_cast< Uint32 >( inputDescs.size() ) };

            D3DX11_PASS_DESC passDesc{};
            pShader->pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
            HRESULT result = pDevice->CreateInputLayout( inputDescs.data(),
                count,
                passDesc.pIAInputSignature,
                passDesc.IAInputSignatureSize,
                &pShader->pInputLayout );

            if ( FAILED( result ) )
                SM_LOG_ERROR( "DirectX11Device::BuildInputLayout > Failed to create input layout: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }

        static void BuildInputLayout( ID3D11Device *pDevice, const memory::Ref< DirectX11Shader > &pShader )
        {
            D3DX11_PASS_SHADER_DESC passShaderDesc{};
            pShader->pTechnique->GetPassByIndex( 0 )->GetVertexShaderDesc( &passShaderDesc );

            D3DX11_EFFECT_SHADER_DESC effectShaderDesc{};
            passShaderDesc.pShaderVariable->GetShaderDesc( passShaderDesc.ShaderIndex, &effectShaderDesc );

            D3D11_SIGNATURE_PARAMETER_DESC signatureParameterDesc{};
            std::vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
            Uint32 stride = 0;

            for ( Uint32 i{}; i < effectShaderDesc.NumInputSignatureEntries; ++i )
            {
                passShaderDesc.pShaderVariable->GetInputSignatureElementDesc(
                    passShaderDesc.ShaderIndex, i, &signatureParameterDesc );

                Uint32 offset = static_cast< Uint32 >( floor( log( signatureParameterDesc.Mask ) / log( 2 ) ) + 1 ) * 4;
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

                VertexParameter element{ DirectXBaseTypeToShaderDataType( type ), signatureParameterDesc.SemanticName };
                pShader->BufferLayout.AddElement( element );

                stride += offset;
            }

            Uint32 count{ static_cast< Uint32 >( inputDescs.size() ) };

            D3DX11_PASS_DESC passDesc{};
            pShader->pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
            HRESULT result = pDevice->CreateInputLayout( inputDescs.data(),
                count,
                passDesc.pIAInputSignature,
                passDesc.IAInputSignatureSize,
                &pShader->pInputLayout );

            if ( FAILED( result ) )
                SM_LOG_ERROR( "DirectX11Shader::BuildInputLayout > Failed to create input layout: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }
    }

    DirectX11Device::DirectX11Device()
    {
        // Create DXGI Factory to create SwapChain based on hardware
        HRESULT result = CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &m_pDXGIFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11Device > Failed to create DXGIFactory: {}", fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }

        // Create Device and Device context, using hardware acceleration
        D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
        Uint32 createDeviceFlags{ 0 };
#ifdef SM_C_DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        result = D3D11CreateDevice( 0,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            createDeviceFlags,
            0,
            0,
            D3D11_SDK_VERSION,
            &m_pInternal,
            &featureLevel,
            &m_pContext );

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11Device > Failed to create D3D11Device: {}", fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }
    }

    DirectX11Device::~DirectX11Device()
    {
        if ( m_pContext )
        {
            m_pContext->ClearState();
            m_pContext->Flush();
            SAFE_RELEASE( m_pContext );
        }

        SAFE_RELEASE( m_pInternal );
    }

    GraphicsContext *DirectX11Device::CreateGraphicsContext()
    {
        auto pGraphicsContext = CreateScope< DirectX11Context >( this, m_pContext );
        DirectX11Context *pRawGraphicsContext = pGraphicsContext.get();
        m_pGraphicsContexts.emplace_back( std::move( pGraphicsContext ) );

        return pRawGraphicsContext;
    }

    memory::Ref< SwapChain > DirectX11Device::CreateSwapChain( const window::Window *pWindow )
    {
        auto pSwapChain = memory::CreateRef< DirectX11SwapChain >( pWindow, m_pInternal, m_pContext, m_pDXGIFactory );
        pSwapChain->Create();
        return pSwapChain;
    }

    void DirectX11Device::CreateGPUBuffer( GPUBufferHandle handle, const GPUBufferDescriptor &bufferDesc )
    {
        m_GPUBuffers[handle.GetIndex()].Create( m_pInternal, bufferDesc );
    }

    void DirectX11Device::DestroyGPUBuffer( GPUBufferHandle handle )
    {
        m_GPUBuffers[handle.GetIndex()].Destroy();
    }

    memory::Ref< Shader > DirectX11Device::CreateShader( const std::string &assetFile,
        const VertexLayout &layout,
        const std::string &techniqueName )
    {
        memory::Ref< DirectX11Shader > pShader = memory::CreateRef< DirectX11Shader >( this );
        pShader->SetName( assetFile );
        pShader->BufferLayout = layout;

        if ( !shaderhelpers::LoadEffect( m_pInternal, pShader, assetFile ) )
        {
            SAFE_RELEASE( pShader->pEffect );
            SM_ASSERT( false, "DirectX11Device::CreateShader > Failed to load effect" );
        }

        if ( !techniqueName.empty() )
            pShader->pTechnique = pShader->pEffect->GetTechniqueByName( techniqueName.c_str() );
        else
            pShader->pTechnique = pShader->pEffect->GetTechniqueByIndex( 0 );

        if ( !pShader->pTechnique->IsValid() )
            SM_LOG_WARNING( "DirectX11Device::CreateShader > Invalid technique" );

        shaderhelpers::BuildInputLayout( m_pInternal, pShader, layout );

        return pShader;
    }

    memory::Ref< Shader > DirectX11Device::CreateShader( const std::string &assetFile,
        const std::string &techniqueName )
    {
        memory::Ref< DirectX11Shader > pShader = memory::CreateRef< DirectX11Shader >( this );
        pShader->SetName( assetFile );

        if ( !shaderhelpers::LoadEffect( m_pInternal, pShader, assetFile ) )
        {
            SAFE_RELEASE( pShader->pEffect );
            SM_ASSERT( false, "DirectX11Device::CreateShader > Failed to load effect" );
        }

        if ( !techniqueName.empty() )
            pShader->pTechnique = pShader->pEffect->GetTechniqueByName( techniqueName.c_str() );
        else
            pShader->pTechnique = pShader->pEffect->GetTechniqueByIndex( 0 );

        if ( !pShader->pTechnique->IsValid() )
            SM_LOG_WARNING( "DirectX11Device::CreateShader > Invalid technique" );

        shaderhelpers::BuildInputLayout( m_pInternal, pShader );

        return pShader;
    }

    void DirectX11Device::CreateTexture( TextureHandle handle, const std::filesystem::path &path )
    {
        m_Textures[handle.GetIndex()].Create( m_pInternal, path );
    }

    void DirectX11Device::CreateTexture( TextureHandle handle, memory::Ref< const Image > pImage )
    {
        m_Textures[handle.GetIndex()].Create( m_pInternal, std::move( pImage ) );
    }

    void DirectX11Device::DestroyTexture( TextureHandle handle )
    {
        m_Textures[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::CreateFramebuffer( FramebufferHandle handle, const FramebufferDescriptor &descriptor )
    {
        m_Framebuffers[handle.GetIndex()].Create( m_pInternal, descriptor );
    }

    void DirectX11Device::DestroyFramebuffer( FramebufferHandle handle )
    {
        m_Framebuffers[handle.GetIndex()].Destroy();
    }

    void DirectX11Device::InvalidateFramebuffer( FramebufferHandle handle )
    {
        m_Framebuffers[handle.GetIndex()].Invalidate( m_pInternal );
    }

    const DirectX11RasterizerState *DirectX11Device::GetOrCreateRasterizerState( const RenderState &renderState )
    {
        const DirectX11RasterizerState *pRasterizerState = m_RasterizerStateCache.Find( renderState );
        if ( pRasterizerState )
            return pRasterizerState;

        auto pNewRasterizerState = CreateScope< DirectX11RasterizerState >();
        pNewRasterizerState->Create( m_pInternal, renderState );
        return m_RasterizerStateCache.Add( renderState, std::move( pNewRasterizerState ) );
    }

    const DirectX11DepthStencilState *DirectX11Device::GetOrCreateDepthStencilState( const RenderState &renderState )
    {
        const DirectX11DepthStencilState *pDepthStencilState = m_DepthStencilStateCache.Find( renderState );
        if ( pDepthStencilState )
            return pDepthStencilState;

        auto pNewDepthStencilState = CreateScope< DirectX11DepthStencilState >();
        pNewDepthStencilState->Create( m_pInternal, renderState );
        return m_DepthStencilStateCache.Add( renderState, std::move( pNewDepthStencilState ) );
    }
}