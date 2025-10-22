/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Matrices : register(b0)
{
    float4x4 World;
    float4x4 ViewProjection;
};

struct VS_IN
{
    float3 PosL : POSITION;
};

struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float3 TexC : TEXCOORD0;
};

VS_OUT VSMain(VS_IN input)
{
    VS_OUT output;

    float4x4 worldViewProjection = mul(World, ViewProjection);

    // Project to far plane (z = w) and use local position for cubemap lookup
    output.PosH = mul(float4(input.PosL, 0.0f), worldViewProjection).xyww;
    output.TexC = input.PosL;

    return output;
}