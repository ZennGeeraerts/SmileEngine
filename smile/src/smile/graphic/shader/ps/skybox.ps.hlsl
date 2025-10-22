/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

TextureCube CubeMap : register(t0);
SamplerState SamLinear : register(s0);

struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float3 TexC : TEXCOORD0;
};

float4 PSMain(VS_OUT input) : SV_TARGET
{
    return CubeMap.Sample(SamLinear, input.TexC);
}