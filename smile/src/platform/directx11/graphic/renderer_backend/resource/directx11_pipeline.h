/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Pipeline final
    {
        ID3D11InputLayout *pInputLayout;

        D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology;
        ID3D11RasterizerState *pRasterizerState;
        ID3D11DepthStencilState *pDepthStencilState;

        ID3D11VertexShader *pVertexShader;
        ID3D11PixelShader *pPixelShader;
    };
}