/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

cbuffer Matrices : register( b0 )
{
    float4x4 World;
    float4x4 ViewProjection;
};

cbuffer SkinnedMatrices : register( b1 )
{
    float4x4 Bones[70];
}

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 Tangent : TANGENT;
    float4 BlendIndices : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHTS;
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
    VS_OUTPUT output = ( VS_OUTPUT )0;

    float4 originalPos = float4( input.Position, 1.0f );
    float4 transformedPos = 0.0f;
    float3 transformedNormal = 0.0f;

    for ( int i = 0; i < 4; ++i )
    {
        int index = ( int )input.BlendIndices[i];
        if ( index >= 0 && index < 70 )
        {
            transformedPos += input.BlendWeights[i] * mul( originalPos, Bones[index] );
            transformedNormal += input.BlendWeights[i] * mul( input.Normal, ( float3x3 )Bones[index] );
        }
    }

    transformedNormal = normalize( transformedNormal );

    float4x4 worldViewProjection = mul( World, ViewProjection );

    if ( transformedPos.w > 0.0f )
    {
        output.Position = mul( transformedPos, worldViewProjection );
        output.WorldPosition = mul( transformedPos, World );
        output.Normal = normalize( mul( transformedNormal, ( float3x3 )World ) );
    }
    else
    {
        output.Position = mul( float4( input.Position, 1.0f ), worldViewProjection );
        output.WorldPosition = mul( float4( input.Position, 1.0f ), World );
        output.Normal = normalize( mul( input.Normal, ( float3x3 )World ) );
    }

    output.TexCoord = input.TexCoord;
    output.Tangent = normalize( mul( input.Tangent, ( float3x3 )World ) );

    return output;
}