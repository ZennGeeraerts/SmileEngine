/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Matrices : register( b0 )
{
    float4x4 World;
    float4x4 ViewProjection;
}

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLDPOSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 Tangent : TANGENT;
};

VS_OUTPUT VSMain( VS_INPUT input )
{
    VS_OUTPUT output;

    float4x4 worldViewProjection = mul( World, ViewProjection );

    output.Position = mul( float4( input.Position, 1.0f ), worldViewProjection );
    output.WorldPosition = mul( float4( input.Position, 1.0f ), World );
    output.Normal = normalize( mul( ( float3x3 )World, input.Normal ) );
    output.TexCoord = input.TexCoord;
    output.Tangent = normalize( mul( ( float3x3 )World, input.Tangent ) );

    return output;
}