/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Camera : register( b0 )
{
    float4x4 ViewProjection;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_OUTPUT VSMain( VS_INPUT input )
{
    VS_OUTPUT output = ( VS_OUTPUT )0;

    output.Pos = mul( float4( input.Pos, 1 ), ViewProjection );
    output.Color = input.Color;

    return output;
}
