/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Camera : register( b0 )
{
    float4x4 ViewProjection;
};

cbuffer PerObject : register( b1 )
{
    float4x4 WorldTransform;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VSMain( VS_INPUT input )
{
    float4x4 worldViewProjection = mul( WorldTransform, ViewProjection );

    VS_OUTPUT output = ( VS_OUTPUT )0;
    output.Position = mul( float4( input.Position, 1.f ), worldViewProjection );
    output.TexCoord = input.TexCoord;
    return output;
}
