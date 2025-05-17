/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/graphics_pipeline.h"
#include "smile/graphic/renderer_backend/shader/shader_stage.h"
#include "smile/common/foundation/flags.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace smile::graphic
{
    class DirectX11Device;

    struct DirectX11Pipeline final
    {
        void Create( DirectX11Device &device, const GraphicsPipelineDescriptor &desc );

        ID3D11InputLayout *pInputLayout;

        D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology;
        ID3D11RasterizerState *pRasterizerState;
        ID3D11DepthStencilState *pDepthStencilState;

        Microsoft::WRL::ComPtr< ID3D11VertexShader > pVertexShader;
        Microsoft::WRL::ComPtr< ID3D11PixelShader > pPixelShader;
        foundation::Flags< ShaderStage > ShaderMask;
        bool PixelShaderHasUAVs = false;
    };
}