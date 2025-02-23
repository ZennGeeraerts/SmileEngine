/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_device.h"
#include "directx11_context.h"

#include "directx11_diagnostics.h"

#include "resource/directx11_vertex_buffer.h"
#include "resource/directx11_index_buffer.h"
#include "resource/directx11_texture.h"
#include "resource/directx11_frame_buffer.h"
#include "resource/directx11_rasterizer_state.h"

#include "shader/directx11_buffer.h"
#include "shader/directx11_shader.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

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

                    OutputDebugStringW( ss.str().c_str() );
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
            const BufferLayout &layout )
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

                BufferElement element{ DirectXBaseTypeToShaderDataType( type ), signatureParameterDesc.SemanticName };
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

    namespace texturehelpers
    {
        static bool LoadTexture( ID3D11Device *pDevice,
            const std::string &filePath,
            ID3D11Resource **ppResource,
            ID3D11ShaderResourceView **ppShaderResourceView,
            DirectX::TexMetadata &info )
        {
            if ( filePath.find_last_of( '.' ) == std::string::npos )
            {
                SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Invalid file extension: {}", filePath );
                return false;
            }

            std::string fileExtension = filePath.substr( filePath.find_last_of( '.' ) + 1 );
            std::wstring filePathWide = std::wstring{ filePath.begin(), filePath.end() };

            DirectX::ScratchImage image{};

            HRESULT result{ S_OK };
            if ( !_strcmpi( fileExtension.c_str(), "dds" ) )
            {
                result = DirectX::LoadFromDDSFile( filePathWide.c_str(), DirectX::DDS_FLAGS_NONE, &info, image );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Loading from DDS file failed: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return false;
                }
            }
            else if ( !_strcmpi( fileExtension.c_str(), "tga" ) )
            {
                result = DirectX::LoadFromTGAFile( filePathWide.c_str(), &info, image );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Loading from TGA file failed: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return false;
                }
            }
            else
            {
                result = DirectX::LoadFromWICFile( filePathWide.c_str(), DirectX::WIC_FLAGS_NONE, &info, image );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Loading from WIC file failed: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return false;
                }
            }

            result = DirectX::CreateTexture(
                pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), ppResource );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Failed to create texture: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                SAFE_RELEASE( ( *ppResource ) );
                return false;
            }

            result = DirectX::CreateShaderResourceView(
                pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), ppShaderResourceView );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Failed to create shader resource view: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                SAFE_RELEASE( ( *ppShaderResourceView ) );
                return false;
            }

            return true;
        }

        static bool LoadTexture2D( ID3D11Device *pDevice, const memory::Ref< DirectX11Texture2D > &pTexture )
        {
            DirectX::TexMetadata info{};

            return LoadTexture(
                pDevice, pTexture->FilePath, &pTexture->pTexture, &pTexture->pShaderResourceView, info );
        }

        static bool LoadTextureCube( ID3D11Device *pDevice, const memory::Ref< DirectX11TextureCube > &pTexture )
        {
            DirectX::TexMetadata info{};
            info.miscFlags = DirectX::TEX_MISC_TEXTURECUBE;

            return LoadTexture(
                pDevice, pTexture->FilePath, &pTexture->pTexture, &pTexture->pShaderResourceView, info );
        }
    }

    namespace framebufferhelpers
    {
        static DXGI_FORMAT FramebufferTextureFormatToDirectXBaseType( FramebufferTextureFormat format )
        {
            switch ( format )
            {
                case FramebufferTextureFormat::RGBA8:
                    return DXGI_FORMAT_B8G8R8A8_UNORM;
                    break;
                case FramebufferTextureFormat::Depth24Stencil8:
                    return DXGI_FORMAT_D24_UNORM_S8_UINT;
                    break;

                case FramebufferTextureFormat::None:
                default:
                    SM_ASSERT( false,
                        "DirectX11Framebuffer::framebufferTextureFormatToDirectXBaseType > Unknown ShaderDataType" );
                    return DXGI_FORMAT_UNKNOWN;
            }
        }
    }

    namespace rasterizerstatehelpers
    {
        static D3D11_CULL_MODE CullModeToDirectXType( CullMode cullMode )
        {
            switch ( cullMode )
            {
                case CullMode::None:
                    return D3D11_CULL_NONE;
                case CullMode::Front:
                    return D3D11_CULL_FRONT;
                case CullMode::Back:
                    return D3D11_CULL_BACK;
                default:
                    return D3D11_CULL_NONE;
            }
        }

        static D3D11_FILL_MODE FillModeToDirectXType( FillMode fillMode )
        {
            switch ( fillMode )
            {
                case FillMode::Solid:
                    return D3D11_FILL_SOLID;
                case FillMode::WireFrame:
                    return D3D11_FILL_WIREFRAME;
                default:
                    return D3D11_FILL_SOLID;
            }
        }
    }

    DirectX11Device::DirectX11Device( GraphicsContext *pContext )
    {
        // Create Device and Device context, using hardware acceleration
        D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
        Uint32 createDeviceFlags{ 0 };
#ifdef SM_C_DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        DirectX11Context *pDirectX11Context = static_cast< DirectX11Context * >( pContext );

        HRESULT result = D3D11CreateDevice( 0,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            createDeviceFlags,
            0,
            0,
            D3D11_SDK_VERSION,
            &m_pInternal,
            &featureLevel,
            &pDirectX11Context->m_pInternal );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Device::Initialize > Failed to create D3D11Device: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }
    }

    DirectX11Device::~DirectX11Device()
    {
        SAFE_RELEASE( m_pInternal );
    }

    memory::Ref< VertexBuffer > DirectX11Device::CreateVertexBuffer( const VertexBufferDescriptor &vertexBufferDesc )
    {
        memory::Ref< DirectX11VertexBuffer > pVertexBuffer = memory::CreateRef< DirectX11VertexBuffer >();
        pVertexBuffer->Stride = vertexBufferDesc.Stride;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = BufferUsageToDirectXType( vertexBufferDesc.Usage );
        bufferDesc.ByteWidth = vertexBufferDesc.Stride * vertexBufferDesc.Count;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = BufferCPUAccessToDirectXType( vertexBufferDesc.CPUAccess );
        bufferDesc.MiscFlags = 0;

        HRESULT result;
        if ( vertexBufferDesc.pVertices )
        {
            D3D11_SUBRESOURCE_DATA initData = { 0 };
            initData.pSysMem = vertexBufferDesc.pVertices;

            result = m_pInternal->CreateBuffer( &bufferDesc, &initData, &pVertexBuffer->pInternal );
        }
        else
        {
            result = m_pInternal->CreateBuffer( &bufferDesc, nullptr, &pVertexBuffer->pInternal );
        }

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Device::CreateVertexBuffer > Failed to create vertex buffer: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return nullptr;
        }

        return pVertexBuffer;
    }

    memory::Ref< IndexBuffer > DirectX11Device::CreateIndexBuffer( const IndexBufferDescriptor &indexBufferDesc )
    {
        memory::Ref< DirectX11IndexBuffer > pIndexBuffer = memory::CreateRef< DirectX11IndexBuffer >();
        pIndexBuffer->Count = indexBufferDesc.Count;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = BufferUsageToDirectXType( indexBufferDesc.Usage );
        bufferDesc.ByteWidth = sizeof( Uint32 ) * indexBufferDesc.Count;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = { 0 };
        initData.pSysMem = indexBufferDesc.pIndices;

        HRESULT result = m_pInternal->CreateBuffer( &bufferDesc, &initData, &pIndexBuffer->pInternal );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11IndexBuffer > Failed to create index buffer: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return nullptr;
        }

        return pIndexBuffer;
    }

    memory::Ref< Shader > DirectX11Device::CreateShader( const std::string &assetFile,
        const BufferLayout &layout,
        const std::string &techniqueName )
    {
        memory::Ref< DirectX11Shader > pShader = memory::CreateRef< DirectX11Shader >();
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
        memory::Ref< DirectX11Shader > pShader = memory::CreateRef< DirectX11Shader >();
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

    memory::Ref< Texture > DirectX11Device::CreateTexture2D( const std::string &filePath )
    {
        memory::Ref< DirectX11Texture2D > pTexture = memory::CreateRef< DirectX11Texture2D >();
        pTexture->FilePath = filePath;

        if ( !texturehelpers::LoadTexture2D( m_pInternal, pTexture ) )
        {
            SAFE_RELEASE( pTexture->pTexture );
            SAFE_RELEASE( pTexture->pShaderResourceView );
            SM_ASSERT( false, "DirectX11Device::CreateTexture2D > Failed to load texture" );
        }

        auto pD11Tex2D = static_cast< ID3D11Texture2D * >( pTexture->pTexture );
        D3D11_TEXTURE2D_DESC tex2Ddesc;
        pD11Tex2D->GetDesc( &tex2Ddesc );

        pTexture->Width = tex2Ddesc.Width;
        pTexture->Height = tex2Ddesc.Height;

        return pTexture;
    }

    memory::Ref< Texture > DirectX11Device::CreateTextureCube( const std::string &filePath )
    {
        memory::Ref< DirectX11TextureCube > pTexture = memory::CreateRef< DirectX11TextureCube >();
        pTexture->FilePath = filePath;

        if ( !texturehelpers::LoadTextureCube( m_pInternal, pTexture ) )
        {
            SAFE_RELEASE( pTexture->pTexture );
            SAFE_RELEASE( pTexture->pShaderResourceView );
            SM_ASSERT( false, "DirectX11Device::CreateTextureCube > Failed to load texture" );
        }

        auto pD11Tex2D = static_cast< ID3D11Texture2D * >( pTexture->pTexture );
        D3D11_TEXTURE2D_DESC tex2Ddesc;
        pD11Tex2D->GetDesc( &tex2Ddesc );

        pTexture->Width = tex2Ddesc.Width;
        pTexture->Height = tex2Ddesc.Height;

        return pTexture;
    }

    memory::Ref< Framebuffer > DirectX11Device::CreateFramebuffer( const FramebufferDescriptor &descriptor )
    {
        memory::Ref< DirectX11Framebuffer > pFramebuffer = memory::CreateRef< DirectX11Framebuffer >();
        pFramebuffer->Descriptor = descriptor;

        for ( const auto &framebufferTextureData : descriptor.Attachments.Attachments )
        {
            if ( !pFramebuffer->IsDepthFormat( framebufferTextureData.TextureFormat ) )
                pFramebuffer->ColorAttachmentData.emplace_back( framebufferTextureData );
            else
                pFramebuffer->DepthAttachmentData = framebufferTextureData;
        }

        InvalidateFramebuffer( pFramebuffer );

        return pFramebuffer;
    }

    memory::Ref< RasterizerState > DirectX11Device::CreateRasterizerState( const RasterizerStateDescriptor &descriptor )
    {
        memory::Ref< DirectX11RasterizerState > pRasterizerState = memory::CreateRef< DirectX11RasterizerState >();

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.CullMode = rasterizerstatehelpers::CullModeToDirectXType( descriptor.CullMode );
        rasterizerDesc.FillMode = rasterizerstatehelpers::FillModeToDirectXType( descriptor.FillMode );
        rasterizerDesc.DepthClipEnable = descriptor.EnableDepthClip;

        HRESULT result = m_pInternal->CreateRasterizerState( &rasterizerDesc, &pRasterizerState->pInternal );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Device::CreateRasterizerState > Failed to create rasterizer state: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }

        return pRasterizerState;
    }

    void DirectX11Device::InvalidateFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer )
    {
        memory::Ref< DirectX11Framebuffer > pD11Framebuffer = memory::Ref< DirectX11Framebuffer >{ pFramebuffer };

        for ( Uint32 i{}; i < pD11Framebuffer->pRenderTargetViews.size(); ++i )
            SAFE_RELEASE( pD11Framebuffer->pRenderTargetViews[i] );

        for ( Uint32 i{}; i < pD11Framebuffer->pColorAttachments.size(); ++i )
            SAFE_RELEASE( pD11Framebuffer->pColorAttachments[i] );

        for ( Uint32 i{}; i < pD11Framebuffer->pColorShaderResourceViews.size(); ++i )
            SAFE_RELEASE( pD11Framebuffer->pColorShaderResourceViews[i] );

        pD11Framebuffer->pRenderTargetViews.clear();
        pD11Framebuffer->pColorAttachments.clear();
        pD11Framebuffer->pColorShaderResourceViews.clear();

        SAFE_RELEASE( pD11Framebuffer->pDepthStencilAttachment );
        SAFE_RELEASE( pD11Framebuffer->pDepthStencilView );

        // Attachments
        if ( pD11Framebuffer->ColorAttachmentData.size() )
        {
            pD11Framebuffer->pColorAttachments.resize( pD11Framebuffer->ColorAttachmentData.size() );
            pD11Framebuffer->pRenderTargetViews.resize( pD11Framebuffer->ColorAttachmentData.size() );
            pD11Framebuffer->pColorShaderResourceViews.resize( pD11Framebuffer->ColorAttachmentData.size() );

            for ( uint32_t i{}; i < pD11Framebuffer->pColorAttachments.size(); ++i )
            {
                D3D11_TEXTURE2D_DESC textureDesc = {};
                textureDesc.Width = pD11Framebuffer->Descriptor.Width;
                textureDesc.Height = pD11Framebuffer->Descriptor.Height;
                textureDesc.MipLevels = 1;
                textureDesc.ArraySize = 1;
                textureDesc.Format = framebufferhelpers::FramebufferTextureFormatToDirectXBaseType(
                    pD11Framebuffer->ColorAttachmentData[i].TextureFormat );
                textureDesc.SampleDesc.Count = pD11Framebuffer->Descriptor.Samples;
                textureDesc.SampleDesc.Quality = 0;
                textureDesc.Usage = D3D11_USAGE_DEFAULT;
                textureDesc.BindFlags =
                    D3D11_BIND_RENDER_TARGET |
                    ( ( pD11Framebuffer->ColorAttachmentData[i].UseInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
                textureDesc.CPUAccessFlags = 0;
                textureDesc.MiscFlags = 0;

                HRESULT result =
                    m_pInternal->CreateTexture2D( &textureDesc, nullptr, &pD11Framebuffer->pColorAttachments[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Device::InvalidateFramebuffer > Failed to create Texture2D: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

                D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
                renderTargetViewDesc.Format = textureDesc.Format;
                renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

                result = m_pInternal->CreateRenderTargetView( pD11Framebuffer->pColorAttachments[i],
                    &renderTargetViewDesc,
                    &pD11Framebuffer->pRenderTargetViews[i] );
                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Device::InvalidateFramebuffer > Failed to create render target view: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

                if ( pD11Framebuffer->ColorAttachmentData[i].UseInShader )
                {
                    result = m_pInternal->CreateShaderResourceView( pD11Framebuffer->pColorAttachments[i],
                        nullptr,
                        &pD11Framebuffer->pColorShaderResourceViews[i] );
                    if ( FAILED( result ) )
                    {
                        SM_LOG_ERROR(
                            "DirectX11Device::InvalidateFramebuffer > Failed to create shader resource view: {}",
                            fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                        return;
                    }
                }
            }
        }

        if ( pD11Framebuffer->DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
        {
            D3D11_TEXTURE2D_DESC depthStencilDesc{};
            depthStencilDesc.Width = pD11Framebuffer->Descriptor.Width;
            depthStencilDesc.Height = pD11Framebuffer->Descriptor.Height;
            depthStencilDesc.MipLevels = 1;
            depthStencilDesc.ArraySize = 1;
            depthStencilDesc.Format = framebufferhelpers::FramebufferTextureFormatToDirectXBaseType(
                pD11Framebuffer->DepthAttachmentData.TextureFormat );
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;
            depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
            depthStencilDesc.BindFlags =
                D3D11_BIND_DEPTH_STENCIL |
                ( ( pD11Framebuffer->DepthAttachmentData.UseInShader ) ? D3D11_BIND_SHADER_RESOURCE : 0 );
            depthStencilDesc.CPUAccessFlags = 0;
            depthStencilDesc.MiscFlags = 0;

            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
            depthStencilViewDesc.Format = depthStencilDesc.Format;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0;

            HRESULT result =
                m_pInternal->CreateTexture2D( &depthStencilDesc, 0, &pD11Framebuffer->pDepthStencilAttachment );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::InvalidateFramebuffer > Failed to create depth stencil buffer: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return;
            }

            result = m_pInternal->CreateDepthStencilView(
                pD11Framebuffer->pDepthStencilAttachment, &depthStencilViewDesc, &pD11Framebuffer->pDepthStencilView );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::InvalidateFramebuffer > Failed to create depth stencil view: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return;
            }
        }

        /*D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = m_Data.Width;
        textureDesc.Height = m_Data.Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_pColorBuffer);
        if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create Texture2D");
            return;
        }

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

        result = m_pDirectX11Context->GetDevice()->CreateRenderTargetView(m_pColorBuffer, &renderTargetViewDesc,
        &m_pRenderTargetView); if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create render target view");
            return;
        }

        result = m_pDirectX11Context->GetDevice()->CreateShaderResourceView(m_pColorBuffer, nullptr,
        &m_pColorShaderResourceView); if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create shader resource view");
            return;
        }

        D3D11_TEXTURE2D_DESC depthStencilDesc{};
        depthStencilDesc.Width = m_Data.Width;
        depthStencilDesc.Height = m_Data.Height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
        depthStencilViewDesc.Format = depthStencilDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

        result = m_pDirectX11Context->GetDevice()->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
        if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create depth stencil buffer");
            return;
        }

        result = m_pDirectX11Context->GetDevice()->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc,
        &m_pDepthStencilView); if (FAILED(result))
        {
            SM_LOG_ERROR("DirectX11Framebuffer::Invalidate > Failed to create depth stencil view");
            return;
        }*/

        auto &viewPort = pD11Framebuffer->Viewport;
        viewPort.Width = static_cast< FLOAT >( pD11Framebuffer->Descriptor.Width );
        viewPort.Height = static_cast< FLOAT >( pD11Framebuffer->Descriptor.Height );
        viewPort.MinDepth = 0.0f;
        viewPort.MaxDepth = 1.0f;
        viewPort.TopLeftX = 0.0f;
        viewPort.TopLeftY = 0.0f;
    }

    void
    DirectX11Device::ResizeFramebuffer( const memory::Ref< Framebuffer > &pFramebuffer, Uint32 width, Uint32 height )
    {
        if ( ( width <= 0 ) || ( height <= 0 ) || ( width > pFramebuffer->MaxFramebufferSize ) ||
             ( height > pFramebuffer->MaxFramebufferSize ) )
        {
            SM_LOG_WARNING( "DirectX11Device::ResizeFramebuffer > Invalid framebuffer size: {0}, {1}", width, height );
            return;
        }

        pFramebuffer->Descriptor.Width = width;
        pFramebuffer->Descriptor.Height = height;

        InvalidateFramebuffer( pFramebuffer );
    }
}